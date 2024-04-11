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
            showPlayers();

        }

        /// <summary>
        /// this function show the players and the admin.
        /// </summary>
        private void showPlayers()
        {

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
                getPlayersJsonObj playersObj = JsonConvert.DeserializeObject<getPlayersJsonObj>(jsonString);

                if (playersObj.Players.Length > 0)
                {
                    // split the players into players array so i could get to them.
                    string[] playersStrings = playersObj.Players.Split(',');

                    // if the current user is the admin he will be able to start the game.
                    if (playersStrings[0] == currentLoggedUser.getUsername)
                    {
                        this.startGame.Visibility = Visibility.Visible;
                        this.admin = playersStrings[0];
                    }

                    // add the admin in the first.
                    string playersInRoom = "Admin: " + this.admin + "\n";
                    playersInRoom += "Players: \n";

                    // add all the rest of the players.
                    for (int i = 2; i < playersStrings.Length; i++)
                    {
                        playersInRoom += playersStrings[i];
                        playersInRoom += "\n";

                    }

                    this.playersBox.Text = playersInRoom; // set the players in the appropriate box.
                }
            }
            else
            {
                lobbyWin.Show();
                this.Close();
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

        /// <summary>
        /// works only for admin, starts a game.
        /// </summary>
        private void StartGameButton(object sender, RoutedEventArgs e)
        {

        }
    }
}
