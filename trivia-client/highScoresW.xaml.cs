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
    /// Interaction logic for highScoresW.xaml
    /// </summary>
    public partial class highScoresW : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window statsMenuW;
        public highScoresW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window statsMenuW)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.statsMenuW = statsMenuW;
            InitializeComponent();
            showHighScores(); 
        }

        /// <summary>
        /// shows the highest scores.
        /// </summary>
        private void showHighScores()
        {
            List<byte> buffer = new List<byte>();
            // build packet content.
            buffer.Add((byte)Codes.HIGH_SCORE_REQUEST);
            buffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0));

            // send the data to server
            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray());

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);

            if ((int)response[0] == Codes.GET_HIGH_SCORE_RESPONSE)
            {
                byte[] bsonData = PacketBuilder.deserializeToData(response); // take only the bson part of the server response.

                // Convert BSON byte array to BsonDocument
                BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(bsonData);

                string jsonString = bsonDocument.ToJson(); // convert to json string

                if(jsonString.Length > 0)
                {
                    //convert the json to object
                    getHighScoresJsonObj stats = JsonConvert.DeserializeObject<getHighScoresJsonObj>(jsonString);

                    string[] statsString = stats.HighScores.Split(',');

                    string scores = "";
                    foreach (string s in statsString)
                    {
                        scores += s;
                        scores += "\n";
                    }

                    this.statBox.Text = scores;
                }
                else
                {
                    this.statBox.Text = "No top 5 :(";
                }
            }
            else
            {
                this.statBox.Text = "No top 5 :(";
            }
        }

        /// <summary>
        /// goes back to stat menu
        /// </summary>
        private void goBackBtn(object sender, RoutedEventArgs e)
        {
            this.statsMenuW.Show();
            this.Close();
        }
    }
}
