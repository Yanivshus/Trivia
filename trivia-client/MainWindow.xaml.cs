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
        user currentLoggedUser;
        public MainWindow()
        {

            this.tcpClient = new TcpClient();
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 9999);
            this.tcpClient.Connect(serverEndPoint);
            this.clientStream = this.tcpClient.GetStream();
            InitializeComponent();

        }

        private void loginToServer(object sender, RoutedEventArgs e)
        {
            //get username and password the user entered.
            string username = this.UsernameBox.Text;
            string userPassword = this.PasswordBox.Text;
            
            loggedUser currTry = new loggedUser { password = userPassword, username = username };// serialize to json.
            string jsonData = JsonConvert.SerializeObject(currTry);// sirialize the object.

            List<byte> buffer = PacketBuilder.BuildPacket(jsonData, Codes.LOGIN_REQUEST);// build to packet i will send to server.

            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray()); // send the buffer to socket.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);// get data from socket.
            //check the code.
            if ((int)response[0] == Codes.LOGIN_RESPONSE)
            {
                byte[] data = PacketBuilder.deserializeToData(response);
                // Convert BSON byte array to BsonDocument
                BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(data);

                // Convert BsonDocument to JSON string
                string jsonString = bsonDocument.ToJson();
                errorBox.Text = jsonString;
                currentLoggedUser = new user(username, userPassword);
            }
            else
            {
                errorBox.Text = "There was an error signing in!";
            }
        }

        private void signupBtn_Click(object sender, RoutedEventArgs e)
        {
            signupW signupWindow = new signupW(this.tcpClient, this.clientStream, this);
            signupWindow.Show();
            // Close the current window if needed
            this.Hide(); // Assuming this method is within a Window
        }
    }
}
