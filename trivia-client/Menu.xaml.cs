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
    /// Interaction logic for Menu.xaml
    /// </summary>
    public partial class Menu : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        user currentLoggedUser;
        Window loginW;
        public Menu(TcpClient tcpClient, NetworkStream clientStream, user currentLoggedUser, Window loginW)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;   
            this.currentLoggedUser = currentLoggedUser;
            this.loginW = loginW;
            InitializeComponent();
            this.UserBox.Text = currentLoggedUser.getUsername;
        }

        private void CreateRoomBtn_Click(object sender, RoutedEventArgs e)
        {

        }

        private void JoinRoomBtn_Click(object sender, RoutedEventArgs e)
        {

        }

        private void StatBtn_Click(object sender, RoutedEventArgs e)
        {

        }

        private void LogoutBtn_Click(object sender, RoutedEventArgs e)
        {
            List<byte> buffer = new List<byte>();
            buffer.Add((byte)Codes.LOGOUT_REQUSET);
            buffer.AddRange(PacketBuilder.CreateDataLengthAsBytes(0)); // the data will be with size 0 , only the code matter.

            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray()); // send data to server.

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);// get the response.

            if ((int)response[0] == Codes.LOGIN_RESPONSE)
            {
                // if the logout was succeful we will return to the main menu which is login.
                MainWindow main = new MainWindow();
                main.Show();
                this.Close();
            }
        }
    }
}
