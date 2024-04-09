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
using MongoDB.Bson.Serialization;
using MongoDB.Bson;
using Newtonsoft.Json;

namespace trivia_client
{
    /// <summary>
    /// Interaction logic for signupW.xaml
    /// </summary>
    public partial class signupW : Window
    {
        TcpClient tcpClient;
        NetworkStream clientStream;
        Window mainWindow;
        public signupW(TcpClient tcpClient, NetworkStream clientStream, Window mainWindow)
        {
            this.tcpClient = tcpClient;
            this.clientStream = clientStream;
            this.mainWindow = mainWindow;
            InitializeComponent();
        }

        private void goBackClick(object sender, RoutedEventArgs e)
        {
            this.mainWindow.Show();
            // Close the current window if needed
            Close(); // Assuming this method is within a Window
        }

        private void signUpAuser(object sender, RoutedEventArgs e)
        {
            string username = this.UsernameBox.Text;
            string password = this.PasswordBox.Text;
            string email = this.emailBox.Text;

            // create a signup object so i could serialize it.
            signUpUser currTry = new signUpUser { username = username, password = password, email = email };
            string jsonData = JsonConvert.SerializeObject(currTry);// parse to json

            // build the packet into list of bytes.
            List<byte> buffer = PacketBuilder.BuildPacket(jsonData, Codes.SIGNUP_REQUEST);

            //send the buffer to the server.
            PacketBuilder.sendDataToSocket(clientStream, buffer.ToArray());

            byte[] response = PacketBuilder.getDataFromSocket(clientStream);

            if ((int)response[0] == Codes.SIGNUP_RESPONSE)
            {
                errorBox.Text = "User signed successfully.";
            }
            else
            {
                errorBox.Text = "There was an error signing up!";
            }


        }
    }
}
