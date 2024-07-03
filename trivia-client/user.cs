using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace trivia_client
{
    public class user
    {
        private string username;
        private string password;
        public user(string username, string password) 
        { 
            this.username = username;
            this.password = password;
        }

        public string getUsername { get { return username; } }
        public string getPassword { get { return password;} }

    }
}
