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
using System.Threading;

namespace trivia_client
{
    /// <summary>
    /// Interaction logic for JoinChooseRoomW.xaml
    /// </summary>
    public partial class JoinChooseRoomW : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window menuW;

        private bool stopBackgroundTask = false;//to stop the showPlayer task.

        public JoinChooseRoomW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window menuW)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;   
            this.currentLoggedUser = currentLoggedUser;
            this.menuW = menuW;
            InitializeComponent();

            StartBackgroundTask();
            showRoom();// start the background task to show the avliable room

           
        }

        /// <summary>
        /// refreshes the active rooms every 3 seconds.
        /// </summary>
        private async void showRoom()
        {
            await Task.Run(async () =>
            {
                while(stopBackgroundTask == false)
                {
                    // building the packet to get rooms.
                    List<byte> getRoomsBuffer = new List<byte>();
                    getRoomsBuffer.Add((byte)Codes.GET_ROOMS_REQUEST);// add appropriate code.
                    getRoomsBuffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0)); // add size of 0 to the packet.

                    PacketBuilder.sendDataToSocket(clientStream, getRoomsBuffer.ToArray());// send the packet.

                    //get back the response of the server.
                    byte[] response = PacketBuilder.getDataFromSocket(clientStream);

                    //check if the request worked.
                    if ((int)response[0] == Codes.GET_ROOMS_RESPONSE)
                    {
                        byte[] bsonData = PacketBuilder.deserializeToData(response); // take only the bson part of the server response.

                        // Convert BSON byte array to BsonDocument
                        BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(bsonData);

                        string jsonString = bsonDocument.ToJson(); // convert to json string

                        //convert the json to object
                        GetRoomJsonObj roomResponseObj = JsonConvert.DeserializeObject<GetRoomJsonObj>(jsonString);

                        //check if there are avliable rooms.
                        if (roomResponseObj.Rooms.Length > 0)
                        {
                            await Dispatcher.InvokeAsync(() =>
                            { 
                                // set the text box to show the rooms.
                                this.roomsBox.Text = roomResponseObj.Rooms;
                            });
                        }
                        else
                        {
                            await Dispatcher.InvokeAsync(() =>
                            {
                                // set the text box to show the rooms.
                                this.roomsBox.Text = "No rooms avaliable :(";
                            });
                        }

                    }
                    else
                    {
                        await Dispatcher.InvokeAsync(() =>
                        {
                            // set the text box to show the rooms.
                            this.roomsBox.Text = "No rooms avaliable :(";
                        });
                    }
                    await Task.Delay(3000);
                }
                

            });
        }

        /// <summary>
        /// stop the backgound rooms.
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

        private void goBackBtn(object sender, RoutedEventArgs e)
        {
            StopBackgroundTask();
            Thread.Sleep(1000);

            this.menuW.Show();
            this.Close();
        }

        private void joinRoomBtn(object sender, RoutedEventArgs e)
        {
            StopBackgroundTask();
            Thread.Sleep(1000);

            int id = int.Parse(this.idBox.Text);

            roomJsonObj roomToJoin = new roomJsonObj { roomId = id }; // create room object
            string jsonData = JsonConvert.SerializeObject(roomToJoin); // serialize to json.

            // build the join room packet.
            List<byte> buffer = PacketBuilder.BuildPacket(jsonData, Codes.JOIN_ROOM_REQUEST);

            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray());// send packet.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);// get reponse from server.

            //check if joined to room.
            if ((int)response[0] == Codes.JOIN_ROOM_RESPONSE)
            {
                lobbyW lobbyWin = new lobbyW(tcpClient, clientStream, currentLoggedUser, this, id);
                this.Hide();
                lobbyWin.Show();

            }
            else
            {
                this.errBox.Text = "FAILED TO JOIN ROOM";
                Thread.Sleep(2000);
                StartBackgroundTask();
                showRoom();
            }


        }
    }
}
