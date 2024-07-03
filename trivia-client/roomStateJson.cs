using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace trivia_client
{
    public class roomStateJson
    {
        public int status {  get; set; }

        public bool hasGameBegun { get; set; }

        public string players { get; set; }

        public int questionCount { get; set; }

        public int answerTimeOut { get; set; }
    }
}
