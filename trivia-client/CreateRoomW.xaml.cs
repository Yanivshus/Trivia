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
using MongoDB.Bson.Serialization;
using MongoDB.Bson;
using Newtonsoft.Json;

namespace trivia_client
{
    /// <summary>
    /// Interaction logic for CreateRoomW.xaml
    /// </summary>
    public partial class CreateRoomW : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window menuW;
        Window LoginW;
        public CreateRoomW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window menuW, Window loginW)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.menuW = menuW;
            this.LoginW = loginW;
            InitializeComponent();
            
        }

        private void CreateRoomBtn(object sender, RoutedEventArgs e)
        {
            // create a room object so i could parse it to json.
            Room newRoom = new Room { answerTime = int.Parse(this.ansTimeBox.Text), maxUsers = int.Parse(this.maxUsersBox.Text) , questionCount = int.Parse(this.qCountBox.Text) , roomName = this.roomNameBox.Text};
            string jsonData = JsonConvert.SerializeObject(newRoom);

            // build the packet with the correct codes.
            List<byte> buffer = PacketBuilder.BuildPacket(jsonData, Codes.CREATE_ROOM_REQUEST);

            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray()); // send to client.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);

            if ((int)response[0] == Codes.CREATE_ROOM_RESPONSE)
            {
                byte[] bsonData = PacketBuilder.deserializeToData(response); // take only the bson part of the server response.

                // Convert BSON byte array to BsonDocument
                BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(bsonData);

                string jsonString = bsonDocument.ToJson(); // convert to json string

                //convert the json to object
                roomJsonObj roomResponseObj = JsonConvert.DeserializeObject<roomJsonObj>(jsonString);

                // after we create a room we enter it as an admin.
                lobbyW lobbyWin = new lobbyW(tcpClient, clientStream, currentLoggedUser, roomResponseObj.roomId, menuW, LoginW);
                lobbyWin.Show();
                this.Close();
                
            }
            else
            {
                this.errorBox.Text = "Create room failed.";
            }
        }

        private void GoBackButton(object sender, RoutedEventArgs e)
        {
            menuW.Show();
            this.Close();
        }
    }
}
