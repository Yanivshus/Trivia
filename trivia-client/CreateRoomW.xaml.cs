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
    /// Interaction logic for CreateRoomW.xaml
    /// </summary>
    public partial class CreateRoomW : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window menuW;
        public CreateRoomW(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window menuW)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.currentLoggedUser = currentLoggedUser;
            this.menuW = menuW;
            InitializeComponent();

        }

        private void CreateRoomBtn(object sender, RoutedEventArgs e)
        {

        }

        private void GoBackButton(object sender, RoutedEventArgs e)
        {
            menuW.Show();
            this.Close();
        }
    }
}
