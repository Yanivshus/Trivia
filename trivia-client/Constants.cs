using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace trivia_client
{
    public static class Codes
    {
		//responses
        public const int LOGIN_REQUEST = 100;
		public const int SIGNUP_REQUEST = 101;
		public const int LOGOUT_REQUSET = 102;
		public const int GET_PLAYERS_IN_ROOM_REQUEST = 103;
		public const int JOIN_ROOM_REQUEST = 104;
		public const int CREATE_ROOM_REQUEST = 105;
		public const int HIGH_SCORE_REQUEST = 106;
		public const int GET_ROOMS_REQUEST = 107;
		public const int GET_PERSONAL_STATS_REQUEST = 108;
		//requests
		public const int ERROR_RESPONSE = 115;
		public const int LOGIN_RESPONSE = 116;
		public const int SIGNUP_RESPONSE = 117;
		public const int GET_ROOMS_RESPONSE = 118;
		public const int GET_PLAYERS_IN_ROOM_RESPONSE = 119;
		public const int GET_HIGH_SCORE_RESPONSE = 120;
		public const int GET_PERSONAL_STATS_RESPONSE = 121;
		public const int JOIN_ROOM_RESPONSE = 122;
		public const int CREATE_ROOM_RESPONSE = 123;
		public const int LOGOUT_RESPONSE = 124;

        public const int CODE_SIZE = 4;
    }

    public static class PacketBuilder
    {
        public static List<byte> BuildPacket(string jsonString, int code)
        {
            List<byte> buffer = new List<byte>();

            buffer.Add((byte)code);// the the code of the packet.

            buffer.AddRange(CreateDataLengthAsBytes(jsonString.Length)); // create the length of the json part.

            byte[] jsonBytesArray = Encoding.ASCII.GetBytes(jsonString); // turn the string into bytes.
            buffer.AddRange(jsonBytesArray);// add the byte json data to the buffer.

            return buffer;
        }
        private static List<byte> CreateDataLengthAsBytes(int num)
        {
            List<byte> bytes = new List<byte>(Codes.CODE_SIZE);

            // Extract each byte from the integer
            for (int i = 0; i < Codes.CODE_SIZE; i++)
            {
                bytes.Add((byte)((num >> (i * 8)) & 0xFF)); // Extract each byte
            }

            // Return the list of bytes
            return bytes;
        }
    }
}
