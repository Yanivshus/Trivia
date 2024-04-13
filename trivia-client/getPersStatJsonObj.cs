using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace trivia_client
{
    public class getPersStatJsonObj
    {
        public string AvgAnswerTime {  get; set; }
        public string NumCorrectAnswers { get; set; }
        public string NumTotalAnswers { get; set; }
        public string NumPlayerGames { get; set; }
    }
}
