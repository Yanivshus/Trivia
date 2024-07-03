using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.RightsManagement;
using System.Text;
using System.Threading.Tasks;

namespace trivia_client
{
    public class Question
    {
        public int status {  get; set; }
        public string question { get; set; }
        public string answers { get; set; }
    }
}
