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
        Window mainWin;
        int roomId;
        string admin;
        Window loginWin;
        roomStateJson roomData;

        private bool stopBackgroundTask = false;//to stop the showPlayer task.

        public lobbyW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, int roomId, Window mainWin, Window loginWin)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.roomId = roomId;
            this.mainWin = mainWin;
            this.loginWin = loginWin;
            this.admin = "";

            InitializeComponent();

            getRoomStateFunc(); // get rooom data.

            // set the room data.
            this.DataBox.Text = "User: " + currentLoggedUser.getUsername + "\n" +
                "Question count: " + roomData.questionCount.ToString() + "\n" +
                "Question timeout: " + roomData.answerTimeOut.ToString();



            StartBackgroundTask();
            showPlayers(); // start the backgound task for update the list of active players.
            this.loginWin = loginWin;
        }

        /// <summary>
        /// get the room state and data.
        /// </summary>
        private void getRoomStateFunc()
        {

            //create the packet.
            List<byte> stateBuffer = new List<byte>();
            stateBuffer.Add((byte)Codes.GET_ROOM_STATE_REQUEST);
            stateBuffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0));

            PacketBuilder.sendDataToSocket(clientStream, stateBuffer.ToArray()); // send the packet

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);// receive data from socket.

            // check if the correct response is back.
            if ((int)response[0] == Codes.GET_ROOM_STATE_RESPONSE)
            {
                byte[] bsonData = PacketBuilder.deserializeToData(response); // take only the bson part of the server response.

                // Convert BSON byte array to BsonDocument
                BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(bsonData);

                string jsonString = bsonDocument.ToJson(); // convert to json string

                roomStateJson stateJs = JsonConvert.DeserializeObject<roomStateJson>(jsonString);

                // set the room;
                roomData = stateJs; // set the room data.
            }

        }
        private bool checkIfLeave(byte[] response)
        {
            for (int i = 0; i < response.Length; i++)
            {
                if ((int)response[i] == Codes.LEAVE_ROOM_RESPONSE)
                {
                    return true;
                }
            }
            return false;
        }
        private bool checkIfStart(byte[] response)
        {
            for (int i = 0; i < response.Length; i++)
            {
                if ((int)response[i] == Codes.START_GAME_RESPONSE)
                {
                    return true;
                }
            }
            return false;
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
                    bool gameStart = false;
                    bool roomLeave = false;
                    byte[] response = new byte[1024];

                    while (clientStream.DataAvailable)
                    {
                        response = PacketBuilder.getDataFromSocket(clientStream);
                        if ((int)response[0] == Codes.LEAVE_ROOM_RESPONSE)
                        {
                            roomLeave = true;
                        }
                        else if ((int)response[0] == Codes.START_GAME_RESPONSE)
                        {
                            gameStart = true;
                        }
                    }


                    if ((int)response[0] == Codes.GET_PLAYERS_IN_ROOM_RESPONSE && !roomLeave && !gameStart )
                    {
                        byte[] bsonData = PacketBuilder.deserializeToData(response); // take only the bson part of the server response.

                        // Convert BSON byte array to BsonDocument
                        BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(bsonData);

                        string jsonString = bsonDocument.ToJson(); // convert to json string

                        //convert the json to object
                        getPlayersJsonObj playersObj = JsonConvert.DeserializeObject<getPlayersJsonObj>(jsonString);

                        if (playersObj.Players.Length > 0)
                        {
                            roomData.players = playersObj.Players; // set the current players.
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
                    else if(roomLeave ) // in case of a leave caught it means that the admin closed the room.
                    {
                        // The room got closed by admin.
                        await Dispatcher.InvokeAsync(() =>
                        {
                            MessageBox.Show("The room closed");
                            handleMemberLeave();
                        });
                    }
                    else if (gameStart )// in case of a start game caught it means that the admin started the game.
                    {
                        // The room got closed by admin.
                        await Dispatcher.InvokeAsync(() =>
                        {
                            MessageBox.Show("The Game is starting.");
                            handleStartGame();
                        });
                    }
                    else
                    {
                        // Update the UI every 3 seconds
                        await Dispatcher.InvokeAsync(() =>
                        {
                            this.playersBox.Text = ((int)response[0]).ToString(); // set the players in the appropriate box.
                        });
                    }
                    await Task.Delay(3000);
                }
            });
        }

        /// <summary>
        /// handle start game for a member who forced to start by admin.
        /// </summary>
        private void handleStartGame()
        {
            StopBackgroundTask();
            Thread.Sleep(1000);

            // clean client stream
            while (clientStream.DataAvailable)
            {
                PacketBuilder.getDataFromSocket(clientStream);// get reponse from server.
            }

            Game gameS = new Game(tcpClient, clientStream, currentLoggedUser, roomId, roomData, loginWin);
            gameS.Show();
            this.Close();
            
        }

        /// <summary>
        /// stop the backgound show players.
        /// </summary>
        private void StopBackgroundTask()
        {
            stopBackgroundTask = true;
        }

        /// <summary>
        /// starts the backgound task.
        /// </summary>
        private void StartBackgroundTask()
        {
            stopBackgroundTask = false;
        }


        /// <summary>
        /// function go back to join room screens. and exists the room.
        /// </summary>
        private void leaveRoom(object sender, RoutedEventArgs e)
        {
            LeaveRoomFunc();
        }
         
        /// <summary>
        /// if a clients is forced to leave by close room command this function will make it.
        /// </summary>
        private void handleMemberLeave()
        {
            StopBackgroundTask();
            Thread.Sleep(1000);

            // clean client stream
            while (clientStream.DataAvailable)
            {
                PacketBuilder.getDataFromSocket(clientStream);// get reponse from server.
            }


            // go back to join room window.
            JoinChooseRoomW roomWindowOpen = new JoinChooseRoomW(tcpClient, clientStream, currentLoggedUser, mainWin, loginWin);
            this.Close();
            roomWindowOpen.Show();
        }

        /// <summary>
        /// function go back to join room screens. and exists the room.
        /// </summary>
        private void LeaveRoomFunc()
        {
            StopBackgroundTask();
            Thread.Sleep(1000);

            // clean client stream
            while (clientStream.DataAvailable)
            {
                PacketBuilder.getDataFromSocket(clientStream);// get reponse from server.
            }

            //create the packet.
            List<byte> leaveRoomBuffer = new List<byte>();
            leaveRoomBuffer.Add((byte)Codes.LEAVE_ROOM_REQUEST);
            leaveRoomBuffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0));

            PacketBuilder.sendDataToSocket(clientStream, leaveRoomBuffer.ToArray());//send data to server.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream); // receive server massage.

            //check if leaving worked.
            if ((int)response[0] == Codes.LEAVE_ROOM_RESPONSE)
            {

                JoinChooseRoomW roomWindowOpen = new JoinChooseRoomW(tcpClient, clientStream, currentLoggedUser, mainWin, loginWin);
                this.Close();
                roomWindowOpen.Show();
            }
            else
            {
                StartBackgroundTask();
                showPlayers();
                MessageBox.Show("Problem leaving");
            }
        }
        /// <summary>
        /// works only for admin, starts a game.
        /// </summary>
        private void StartGameButton(object sender, RoutedEventArgs e)
        {
            StopBackgroundTask();
            Thread.Sleep(1000);

            // clean client stream
            while (clientStream.DataAvailable)
            {
                PacketBuilder.getDataFromSocket(clientStream);// get reponse from server.
            }

            //create the packet.
            List<byte> closeBuffer = new List<byte>();
            closeBuffer.Add((byte)Codes.START_GAME_REQUEST);
            closeBuffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0));

            PacketBuilder.sendDataToSocket(clientStream, closeBuffer.ToArray());//send data to server.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream); // receive server massage.

            if ((int)response[0] == Codes.START_GAME_RESPONSE)
            {
                MessageBox.Show("The Game is starting.");

                // start game.
                Game gameS = new Game(tcpClient, clientStream, currentLoggedUser, roomId, roomData, loginWin);
                gameS.Show();
                this.Close();
            }
            else
            {
                StartBackgroundTask();
                showPlayers();              
            }
        }


        /// <summary>
        /// works only for admin, starts a game.
        /// </summary>
        private void closeRoom(object sender, RoutedEventArgs e)
        {
            StopBackgroundTask();
            Thread.Sleep(1000);

            // clean client stream
            while (clientStream.DataAvailable)
            {
                PacketBuilder.getDataFromSocket(clientStream);// get reponse from server.
            }


            //create the packet.
            List<byte> closeBuffer = new List<byte>();
            closeBuffer.Add((byte)Codes.CLOSE_ROOM_REQUEST);
            closeBuffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0));

            PacketBuilder.sendDataToSocket(clientStream, closeBuffer.ToArray());//send data to server.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream); // receive server massage.

            //check if leaving worked.
            if ((int)response[0] == Codes.CLOSE_ROOM_RESPONSE)
            {

                Menu menuW = new Menu(tcpClient, clientStream, currentLoggedUser, mainWin);
                this.Close();
                menuW.Show();
            }
            else
            {
                StartBackgroundTask();
                showPlayers();
                MessageBox.Show("Problem leaving");
            }
        }


    }
}
