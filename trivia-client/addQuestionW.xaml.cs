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
    /// Interaction logic for addQuestionW.xaml
    /// </summary>
    public partial class addQuestionW : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window mainW;
        public addQuestionW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window mainW)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.mainW = mainW;
            InitializeComponent();
        }

        private void addQ(object sender, RoutedEventArgs e)
        {
            addQuestionJson req = new addQuestionJson { question=this.qBox.Text, w_answer1=this.wAns1.Text, w_answer2=this.wAns2.Text, w_answer3=this.wAns3.Text, c_answer4=this.cAns.Text };
            string jsonData = JsonConvert.SerializeObject(req);

            List<byte> buffer = PacketBuilder.BuildPacket(jsonData, Codes.ADD_QUESTION_REQUEST);

            PacketBuilder.sendDataToSocket(clientStream,buffer.ToArray());

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);

            if ((int)response[0] == Codes.ADD_QUESTION_RESPONSE)
            {
                this.mainW.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Problem adding question");
            }
        }

        private void goBack(object sender, RoutedEventArgs e)
        {
            mainW.Show();
            this.Close();            
        }
    }
}
