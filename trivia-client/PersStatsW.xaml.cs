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
    /// Interaction logic for PersStatsW.xaml
    /// </summary>
    public partial class PersStatsW : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window statsMenuW;
        public PersStatsW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window statsMenuW)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.statsMenuW = statsMenuW;
            InitializeComponent();
            this.usernameBox.Text = "Stats: " + currentLoggedUser.getUsername;
            getPersonalStat();
        }

        /// <summary>
        /// function to get the personal stats.
        /// </summary>
        private void getPersonalStat()
        {
            List<byte> buffer = new List<byte>();
            // build packet content.
            buffer.Add((byte)Codes.GET_PERSONAL_STATS_REQUEST);
            buffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0));

            // send the data to server
            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray());

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);

            if ((int)response[0] == Codes.GET_PERSONAL_STATS_RESPONSE) 
            {
                byte[] bsonData = PacketBuilder.deserializeToData(response); // take only the bson part of the server response.

                // Convert BSON byte array to BsonDocument
                BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(bsonData);

                string jsonString = bsonDocument.ToJson(); // convert to json string

                if(jsonString.Length > 0)
                {
                    //convert the json to object
                    getPersStatJsonObj stats = JsonConvert.DeserializeObject<getPersStatJsonObj>(jsonString);

                    //build user stats.
                    string statString = "Avarge answer time: " + stats.AvgAnswerTime + "\n" +
                        "Number of correct Answers: " + stats.NumCorrectAnswers + "\n" +
                        "Number of total answers: " + stats.NumTotalAnswers + "\n" +
                        "Number of player games: " + stats.NumPlayerGames + "\n";
                    this.statBox.Text = statString;
                }
                else
                {
                    this.statBox.Text = "Error loading stats.1";
                }               
            }
            else
            {
                this.statBox.Text = "Error loading stats.2";
            }

        }

        /// <summary>
        /// returns back to the stats choice screen.
        /// </summary>
        private void GoBackBtn(object sender, RoutedEventArgs e)
        {
            statsMenuW.Show();
            this.Close();
        }
    }
}
