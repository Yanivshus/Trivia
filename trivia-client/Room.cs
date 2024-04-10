using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace trivia_client
{
    public class Room
    {
        public int answerTime { get; set; }
        public int maxUsers { get; set; }
        public int questionCount { get; set; }
        public string roomName { get; set; }
    }
}
