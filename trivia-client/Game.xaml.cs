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
using System.Windows.Threading;
using Newtonsoft.Json;

namespace trivia_client
{
    /// <summary>
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : Window
    {
        int gameId = 0;
        roomStateJson roomData;
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Button[] ansButtons = new Button[4];
        int[] ansIdByButton = new int[4]; // to now the id of the answer by the button (answer).

        private DispatcherTimer timer;
        private int totalTime = 10; // Total time in seconds
        private int timeLeft;

        public Game(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, int roomId, roomStateJson roomData)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;   
            this.currentLoggedUser = currentLoggedUser;
            this.gameId = roomId;
            this.roomData = roomData;
            InitializeComponent();

            // set the buttons.
            ansButtons[0] = this.ans1Btn;
            ansButtons[1] = this.ans2Btn;
            ansButtons[2] = this.ans3Btn;
            ansButtons[3] = this.ans4Btn;

            totalTime = this.roomData.answerTimeOut; // set the timout per question.

        }

        // initializes the timer.
        private void StartTimer()
        {
            timeLeft = totalTime;
            UpdateTimerDisplay();
            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Tick += Timer_Tick;
            timer.Start();
        }


        // make a single tick in the timer.
        private void Timer_Tick(object sender, EventArgs e)
        {
            timeLeft--;
            UpdateTimerDisplay();
            if (timeLeft <= 0)
            {
                timer.Stop();
                MessageBox.Show("Time's up!");
                // Add code here to handle when time is up (e.g., move to next question)
            }
        }

        // updated the time on the screem
        private void UpdateTimerDisplay()
        {
            int minutes = timeLeft / 60;
            int seconds = timeLeft % 60;
            timerText.Text = $"Time Left: {minutes:00}:{seconds:00}";
        }

        // restarts the timer.
        private void RestartTimer()
        {
            timer.Stop();
            timeLeft = totalTime;
            UpdateTimerDisplay();
            timer.Start();
        }



        private void leaveGame_Click(object sender, RoutedEventArgs e)
        {

        }

        private void ansBtnClick(object sender, RoutedEventArgs e)
        {
            
        }


        // submit an answer and moves in the server to the next one, 3 is alwayes the correct answer.
        private int submitAnswer(int ansId)
        {
            Ans currAns = new Ans { correctAnsId = ansId };
            string jsonData = JsonConvert.SerializeObject(currAns);

            //build the packet.
            List<byte> buffer = PacketBuilder.BuildPacket(jsonData, Codes.SUBMIT_ANSWER_REQUEST);

            //send to server.
            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray());

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);//get response from server.

            if ((int)response[0] == Codes.SUBMIT_ANSWER_RESPONSE)
            {
                return 3;
            }

            return -1; // problem.
            
        }


        // get question and answers with theyre id.
        private Question getQuestion()
        {
            // build an empty packet to get current question
            List<byte> buffer = new List<byte>();
            buffer.Add((byte)Codes.GET_QUESTION_REQUEST);
            buffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0));

            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray()); // send to server request.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);

            if ((int)response[0] == Codes.GET_QUESTION_RESPONSE)
            {
                byte[] bsonData = PacketBuilder.deserializeToData(response); // take only the bson part of the server response.

                // Convert BSON byte array to BsonDocument
                BsonDocument bsonDocument = BsonSerializer.Deserialize<BsonDocument>(bsonData);

                string jsonString = bsonDocument.ToJson(); // convert to json string

                Question q = JsonConvert.DeserializeObject<Question>(jsonString);

                return q;

            } 
            // if there was a problem getting the question.
            Question qe = new Question();
            qe.status = 0;
            qe.question = "over";
            qe.answers = "";
            return qe;
        }
    }
}
