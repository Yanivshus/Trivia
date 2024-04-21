using MongoDB.Bson.Serialization;
using MongoDB.Bson;
using System;
using System.Collections.Generic;
using System.Linq;
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
using System.Windows.Shapes;
using Newtonsoft.Json;

namespace trivia_client
{
    /// <summary>
    /// Interaction logic for resWin.xaml
    /// </summary>   

    public partial class resWin : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window LoginW;
        int roomId;
        public resWin(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window LoginW, int roomId)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.LoginW = LoginW;
            this.roomId = roomId;
            InitializeComponent();

            int status = 0;
            string gameRes = "";
            while (status == 0)
            {
                //create the packet.
                List<byte> buffer = new List<byte>();
                buffer.Add((byte)Codes.GET_GAME_RESULTS_REQUEST);
                buffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0));

                PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray());//send data to server.

                byte[] response = PacketBuilder.getDataFromSocket(clientStream); // receive server massage.

                if ((int)response[0] == Codes.GET_GAME_RESULTS_RESPONSE)
                {
                    byte[] bsonData = PacketBuilder.deserializeToData(response); // take only the bson part of the server response.

                    // Convert BSON byte array to BsonDocument
                    BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(bsonData);

                    string jsonString = bsonDocument.ToJson(); // convert to json string

                    resultsJsonObj res = JsonConvert.DeserializeObject<resultsJsonObj>(jsonString);

                    status = res.status;
                    gameRes = res.results;

                }
            }

            this.resBox.Text = gameRes;
            this.leaveResBtn.IsEnabled = true;
        }

        private void leaveBtn(object sender, RoutedEventArgs e)
        {
            roomJsonObj delReq = new roomJsonObj { roomId = this.roomId};
            string jsonData = JsonConvert.SerializeObject(delReq);

            List<byte> buffer = PacketBuilder.BuildPacket(jsonData, Codes.DELETE_GAME_REQUEST);

            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray());//send data to server.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream); // receive server massage.

            Menu menuWin = new Menu(tcpClient, clientStream, currentLoggedUser, LoginW);
            menuWin.Show();
            this.Close();
        }
    }
}
