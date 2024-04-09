using System;
using System.Collections.Generic;
using System.IO.Packaging;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using MongoDB.Bson.Serialization;
using MongoDB.Bson;
using Newtonsoft.Json;


namespace trivia_client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        loggedUser currentLoggedUser;
        public MainWindow()
        {
            tcpClient = new TcpClient();
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 9999);
            tcpClient.Connect(serverEndPoint);
            if (!tcpClient.Connected)
            {
                // handle new window of errror later.
            }
            clientStream = tcpClient.GetStream();
            InitializeComponent();
        }

        private void loginToServer(object sender, RoutedEventArgs e)
        {
            //get username  and password the user entered.
            string username = this.UsernameBox.Text;
            string userPassword = this.PasswordBox.Text;

            int currentCode = Codes.LOGIN_REQUEST; // login request code.
            
            loggedUser currUser = new loggedUser { password = userPassword, username = username };// serialize to json.
            string jsonData = JsonConvert.SerializeObject(currUser);// sirialize the object.
            errorBox.Text = jsonData;

            List<byte> buffer = PacketBuilder.BuildPacket(jsonData, Codes.LOGIN_REQUEST);// build to packet i will send to server.

            clientStream.Write(buffer.ToArray(), 0, buffer.Count());// write to the tcp stream the massage.
            clientStream.Flush();

            byte[] response = new byte[1024];
            int bytesRead = clientStream.Read(response, 0, 1024);//get data from tcp stream.
            if (bytesRead > 0)
            {
                //check the code.
                if ((int)response[0] == Codes.LOGIN_RESPONSE)
                {
                    byte[] data = PacketBuilder.deserializeToData(response);
                    // Convert BSON byte array to BsonDocument
                    BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(data);

                    // Convert BsonDocument to JSON string
                    string jsonString = bsonDocument.ToJson();
                    errorBox.Text = jsonString;
                }
                else
                {
                    errorBox.Text = "There was an error signing in!";
                }
            }
        }


    }
}
