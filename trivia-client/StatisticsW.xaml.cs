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
    /// Interaction logic for StatisticsW.xaml
    /// </summary>
    public partial class StatisticsW : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window menuW;
        public StatisticsW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window menuW)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.menuW = menuW;
            InitializeComponent();
            this.userBox.Text = currentLoggedUser.getUsername;
        }

        /// <summary>
        /// shows top 5 highest scores.
        /// </summary>
        private void HighScoreBtn(object sender, RoutedEventArgs e)
        {
            highScoresW highScoreWin = new highScoresW(tcpClient, clientStream, currentLoggedUser, this);
            highScoreWin.Show();
            this.Hide();
        }

        /// <summary>
        /// shows the connected user personal scores.
        /// </summary>
        private void PersScoreBtn(object sender, RoutedEventArgs e)
        {
            PersStatsW persStatWin = new PersStatsW(tcpClient, clientStream, currentLoggedUser, this);
            persStatWin.Show();
            this.Hide();
        }

        /// <summary>
        /// goes back to menu
        /// </summary>
        private void GoBackBtn(object sender, RoutedEventArgs e)
        {
            this.menuW.Show();
            this.Close();
        }
    }
}
