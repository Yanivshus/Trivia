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
using System.Threading;

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

            // initialize the first question.
            StartTimer();
            Question q = getQuestion();
            this.qBox.Text = q.question;
            setAnswersToButtons(q.answers);

        }

        private void setAnswersToButtons(string answers)
        {
            if (answers.Length > 0)
            {
                string[] ansAndId = answers.Split(", ");
                 
                // aplit each answer and its id.
                string[] ans1 = ansAndId[0].Split("=");
                string[] ans2 = ansAndId[1].Split("=");
                string[] ans3 = ansAndId[2].Split("=");
                string[] ans4 = ansAndId[3].Split("=");

                // set the content of each button to be a possiable answer.
                ansButtons[0].Content = ans1[1];
                ansButtons[1].Content = ans2[1];
                ansButtons[2].Content = ans3[1];
                ansButtons[3].Content = ans4[1];

                // set the tag to be the actual id of the answer.
                ansButtons[0].Tag = int.Parse(ans1[0]);
                ansButtons[1].Tag = int.Parse(ans2[0]);
                ansButtons[2].Tag = int.Parse(ans3[0]);
                ansButtons[3].Tag = int.Parse(ans4[0]);
            }
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

        private void setAllToRed()
        {
            ansButtons[0].Background = Brushes.Red;
            ansButtons[1].Background = Brushes.Red;
            ansButtons[2].Background = Brushes.Red;
            ansButtons[3].Background = Brushes.Red;
        }


        private void setAllToWhite()
        {
            ansButtons[0].Background = Brushes.White;
            ansButtons[1].Background = Brushes.White;
            ansButtons[2].Background = Brushes.White;
            ansButtons[3].Background = Brushes.White;
        }


        // make a single tick in the timer.
        private void Timer_Tick(object sender, EventArgs e)
        {
            timeLeft--;
            UpdateTimerDisplay();
            if (timeLeft <= 0)
            {
                timer.Stop();
                submitAnswer(0);
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
            timeLeft = totalTime;
            UpdateTimerDisplay();
            timer.Start();
        }

        private void dissableButtons()
        {
            ansButtons[0].IsEnabled = false;
            ansButtons[1].IsEnabled = false;
            ansButtons[2].IsEnabled = false;
            ansButtons[3].IsEnabled = false;
        }

        private void enableButtons()
        {
            ansButtons[0].IsEnabled = true;
            ansButtons[1].IsEnabled = true;
            ansButtons[2].IsEnabled = true;
            ansButtons[3].IsEnabled = true;
        }

        private void leaveGame_Click(object sender, RoutedEventArgs e)
        {

        }

        private void ansBtnClick(object sender, RoutedEventArgs e)
        {
            setAllToWhite();
            // get all the details from button.
            Button selectedBtn = (Button)sender;
            int answerId = int.Parse(selectedBtn.Tag.ToString());
            string ans = selectedBtn.Content.ToString();

          
            timer.Stop();

            if(submitAnswer(answerId) == answerId)
            {
                selectedBtn.Background = Brushes.Green;
            }
            else
            {
                selectedBtn.Background = Brushes.Red;
            }

            //dissableButtons();
            Thread.Sleep(1000);


            Question q = getQuestion();
            if (q.question != "over")
            {
                this.qBox.Text = q.question;
                enableButtons();
                
                setAnswersToButtons(q.answers);
                RestartTimer();
            }
            else
            {
                MessageBox.Show("Game done for now");
                // move to the results screen.
            }



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
