using System;
using System.Collections.Generic;
using System.Linq;
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
    /// Interaction logic for signupW.xaml
    /// </summary>
    public partial class signupW : Window
    {
        public signupW()
        {
            InitializeComponent();
        }

        private void goBackClick(object sender, RoutedEventArgs e)
        {
            MainWindow loginWindow = new MainWindow();
            loginWindow.Show();
            // Close the current window if needed
            Close(); // Assuming this method is within a Window
        }

        private void signUpAuser(object sender, RoutedEventArgs e)
        {
            //TODO
        }
    }
}
