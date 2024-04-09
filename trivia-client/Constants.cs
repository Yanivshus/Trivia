using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

using MongoDB.Bson;
using MongoDB.Bson.IO;
using MongoDB.Bson.Serialization;

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
        public static byte[] deserializeToData(byte[] packet)
        {
            //slice the length of the data.
            byte[] lengthBytes = new byte[4];
            Array.Copy(packet, 1, lengthBytes, 0, 4);
            int dataLength = GetDataLength(lengthBytes);//get the length of the data.

            byte[] data = new byte[dataLength];
            Array.Copy(packet, 5, data, 0, dataLength);// get data itself from the pakcet.

            return data;
        }

        public static List<byte> BuildPacket(string jsonString, int code)
        {
            List<byte> buffer = new List<byte>();

            buffer.Add((byte)code);// the the code of the packet.

            

            // Parse JSON string to BsonDocument
            BsonDocument bsonDocument = BsonDocument.Parse(jsonString);
            // Convert BsonDocument to BSON byte array
            byte[] bsonBytes = bsonDocument.ToBson();

            buffer.AddRange(CreateDataLengthAsBytes(bsonBytes.Length)); // create the length of the bson part.

            buffer.AddRange(bsonBytes);// add the byte json data to the buffer.

            return buffer;
        }

        private static List<byte> CreateDataLengthAsBytes(int num)
        {
            byte[] bytes = new byte[Codes.CODE_SIZE];

            // Extract each byte from the integer
            for (int i = 0; i < Codes.CODE_SIZE; i++)
            {
                bytes[Codes.CODE_SIZE - 1 - i] = (byte)((num >> (i * 8)) & 0xFF); // Extract each byte
            }
            // Return the vector of bytes
            return bytes.ToList();
        }

        private static int GetDataLength(byte[] bytes)
        {
            int value = 0;

            // Combine each byte into the integer
            for (int i = 0; i < bytes.Length; ++i)
            {
                value |= (int)(bytes[i]) << (8 * (Codes.CODE_SIZE - 1 - i)); // Combine the byte into the integer
            }

            return value;
        }


    }
}
