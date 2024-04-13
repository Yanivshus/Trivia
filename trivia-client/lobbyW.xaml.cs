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
using System.Threading;

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
        Window menuWin;
        int roomId;
        string admin;

        private bool stopBackgroundTask = false;//to stop the showPlayer task.

        public lobbyW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window lobbyWin, int roomId)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.menuWin = lobbyWin;
            this.roomId = roomId;
            this.admin = "";
            InitializeComponent();

            showPlayers(); // start the backgound task for update the list of active players.

        }

        /// <summary>
        /// this function show the players and the admin.
        /// </summary>
        private async void showPlayers()
        {
            await Task.Run(async () =>
            {
                while (stopBackgroundTask == false)
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

                            // Update admin
                            await Dispatcher.InvokeAsync(() =>
                            {
                                // checks if the admin isn't initialized yet.
                                if (admin == "")
                                { 
                                    // if not we will intilize.
                                    this.admin = playersStrings[0]; // the first logged user is alwayes the admin.
                                    // initialize admin ui.
                                    if (admin == currentLoggedUser.getUsername)
                                    {
                                        this.leaveRoomBtn.Visibility = Visibility.Hidden;
                                        this.closeRoomBtn.Visibility = Visibility.Visible;
                                        this.startGame.Visibility = Visibility.Visible;
                                    }
                                   
                                }
                            });
                            

                            // add the admin in the first.
                            string playersInRoom = "Admin: " + this.admin + "\n";
                            playersInRoom += "Players: \n";

                            // add all the rest of the players.
                            for (int i = 1; i < playersStrings.Length; i++)
                            {
                                playersInRoom += playersStrings[i];
                                playersInRoom += "\n";

                            }
                            // Update the UI every 3 seconds
                            await Dispatcher.InvokeAsync(() =>
                            {
                                this.playersBox.Text = playersInRoom; // set the players in the appropriate box.
                            });
                        }
                    }
                    else
                    {
                        // Update the UI every 3 seconds
                        await Dispatcher.InvokeAsync(() =>
                        {
                            this.playersBox.Text = "error"; // set the players in the appropriate box.
                        });
                    }

                    await Task.Delay(3000);
                }
            });
        }

        /// <summary>
        /// stop the backgound show players.
        /// </summary>
        private void StopBackgroundTask()
        {
            stopBackgroundTask = true;
        }


        /// <summary>
        /// function go back to join room screens. TODO, leave room 
        /// </summary>
        private void leaveRoom(object sender, RoutedEventArgs e)
        {
            StopBackgroundTask();
            Thread.Sleep(1000);

            //create the packet.
            List<byte> leaveRoomBuffer = new List<byte>();
            leaveRoomBuffer.Add((byte)Codes.LEAVE_ROOM_REQUEST);
            leaveRoomBuffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0));

            PacketBuilder.sendDataToSocket(clientStream, leaveRoomBuffer.ToArray());//send data to server.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream); // receive server massage.

            //check if leaving worked.
            if ((int)response[0] == Codes.LEAVE_ROOM_RESPONSE)
            {
                
                menuWin.Show();
                this.Close();
            }
            else
            {
                showPlayers();
                MessageBox.Show("Problem leaving");
            }
        }

        /// <summary>
        /// works only for admin, starts a game.
        /// </summary>
        private void StartGameButton(object sender, RoutedEventArgs e)
        {

        }


        /// <summary>
        /// works only for admin, starts a game.
        /// </summary>
        private void closeRoom(object sender, RoutedEventArgs e)
        {

        }
    }
}
