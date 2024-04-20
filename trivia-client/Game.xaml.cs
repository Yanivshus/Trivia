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
        }



        private void leaveGame_Click(object sender, RoutedEventArgs e)
        {

        }

        private void ansBtnClick(object sender, RoutedEventArgs e)
        {

        }
    }
}
