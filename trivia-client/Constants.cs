using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
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
        public const int LOGIN_REQUEST = 80;
        public const int SIGNUP_REQUEST = 81;
        public const int LOGOUT_REQUSET = 82;
        public const int GET_PLAYERS_IN_ROOM_REQUEST = 83;
        public const int JOIN_ROOM_REQUEST = 84;
        public const int CREATE_ROOM_REQUEST = 85;
        public const int HIGH_SCORE_REQUEST = 86;
        public const int GET_ROOMS_REQUEST = 87;
        public const int GET_PERSONAL_STATS_REQUEST = 88;

        public const int CLOSE_ROOM_REQUEST = 89;
        public const int START_GAME_REQUEST = 90;
        public const int GET_ROOM_STATE_REQUEST = 91;
        public const int LEAVE_ROOM_REQUEST = 92;

        public const int GET_GAME_RESULTS_REQUEST = 93;
        public const int SUBMIT_ANSWER_REQUEST = 94;
        public const int GET_QUESTION_REQUEST = 95;
        public const int LEAVE_GAME_REQUEST = 96;
        public const int DELETE_GAME_REQUEST = 97;



        public const int ERROR_RESPONSE = 107;
        public const int LOGIN_RESPONSE = 108;
        public const int SIGNUP_RESPONSE = 109;
        public const int GET_ROOMS_RESPONSE = 110;
        public const int GET_PLAYERS_IN_ROOM_RESPONSE = 111;

        public const int GET_HIGH_SCORE_RESPONSE = 112;
        public const int GET_PERSONAL_STATS_RESPONSE = 113;
        public const int JOIN_ROOM_RESPONSE = 114;
        public const int CREATE_ROOM_RESPONSE = 115;
        public const int LOGOUT_RESPONSE = 116;

        public const int CLOSE_ROOM_RESPONSE = 117;
        public const int START_GAME_RESPONSE = 118;
        public const int GET_ROOM_STATE_RESPONSE = 119;
        public const int LEAVE_ROOM_RESPONSE = 120;

        public const int GET_GAME_RESULTS_RESPONSE = 121;
	    public const int SUBMIT_ANSWER_RESPONSE = 122;
	    public const int GET_QUESTION_RESPONSE = 123;
        public const int LEAVE_GAME_RESPONSE = 124;
        public const int DELETE_GAME_RESPONSE = 125;

        public const int CODE_SIZE = 4;
    }

    public static class PacketBuilder
    {
        /// <summary>
        /// takes a whole packet and returns only the bson.
        /// </summary>
        /// <param name="packet:">the whole packet.</param>
        /// <returns>only the data bson</returns>
        public static byte[] deserializeToData(byte[] packet)
        {
            //slice the length of the data.
            byte[] lengthBytes = new byte[4];
            Array.Copy(packet, 1, lengthBytes, 0, 4); // the size of length of data is 4
            int dataLength = GetDataLength(lengthBytes);//get the length of the data.

            byte[] data = new byte[dataLength];
            Array.Copy(packet, 5, data, 0, dataLength);// get data itself from the pakcet.

            return data;
        }


        /// <summary>
        /// creates a packet by code, dataLength, data.
        /// </summary>
        /// <param name="jsonString:">the data.</param>
        /// <param name="code:">the code of the packet.</param>
        /// <returns>list of bytes which represnts the packet.</returns>
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

        /// <summary>
        /// turn the data length into bytes
        /// </summary>
        /// <param name="num:">size of data</param>
        /// <returns>list of 4 bytes which is the length of data.</returns>
        public static List<byte> CreateDataLengthAsBytes(int num)
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

        /// <summary>
        /// get the length as int from the byte array.
        /// </summary>
        /// <param name="bytes:">bytes array.</param>
        /// <returns>length of data</returns>
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

        /// <summary>
        /// gets data from socket stream.
        /// </summary>
        public static byte[] getDataFromSocket(NetworkStream clientStream)
        {
            byte[] response = new byte[1024];
            int bytesRead = clientStream.Read(response, 0, 1024);//get data from tcp stream.
            return response;
        }


        /// <summary>
        /// send given data to socket stream.
        /// </summary>
        public static void sendDataToSocket(NetworkStream clientStream, byte[] data)
        {
            clientStream.Write(data.ToArray(), 0, data.Count());// write to the tcp stream the massage.
            clientStream.Flush();
        }

    }
}
