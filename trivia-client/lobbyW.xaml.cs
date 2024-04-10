using MongoDB.Bson.Serialization;
using MongoDB.Bson;
using Newtonsoft.Json;
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

namespace trivia_client
{
    /// <summary>
    /// Interaction logic for lobbyW.xaml
    /// </summary>
    public partial class lobbyW : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window lobbyWin;
        int roomId;
        string admin;
        public lobbyW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window lobbyWin, int roomId)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.lobbyWin = lobbyWin;
            this.roomId = roomId;
            InitializeComponent();

            roomJsonObj roomToGetPlayers = new roomJsonObj { roomId = roomId };
            string jsonData = JsonConvert.SerializeObject(roomToGetPlayers);

            // build get players in room packet.
            List<byte> buffer = PacketBuilder.BuildPacket(jsonData, Codes.GET_PLAYERS_IN_ROOM_REQUEST);

            // send data to client.
            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray());

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);

            if ((int)response[0] == Codes.GET_PLAYERS_IN_ROOM_RESPONSE)
            {
                byte[] bsonData = PacketBuilder.deserializeToData(response); // take only the bson part of the server response.

                // Convert BSON byte array to BsonDocument
                BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(bsonData);

                string jsonString = bsonDocument.ToJson(); // convert to json string

                //convert the json to object
                //GetRoomJsonObj roomResponseObj = JsonConvert.DeserializeObject<GetRoomJsonObj>(jsonString);
            }
            else
            {

            }
            


        }

        /// <summary>
        /// function go back to join room screens. TODO, leave room 
        /// </summary>
        private void goBackBtn(object sender, RoutedEventArgs e)
        {
            lobbyWin.Show();
            this.Close();
        }

        private void StartGameButton(object sender, RoutedEventArgs e)
        {

        }
    }
}
