using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace trivia_client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        public MainWindow()
        {
            tcpClient = new TcpClient();
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 9999);
            tcpClient.Connect(serverEndPoint);
            if (!tcpClient.Connected)
            {
                // handle new window of errror later.
            }
            clientStream = tcpClient.GetStream();
            InitializeComponent();
        }

        private void loginToServer(object sender, RoutedEventArgs e)
        {
            string username = this.UsernameBox.Text;
            string userPassword = this.PasswordBox.Text;

            
        }
    }
}
