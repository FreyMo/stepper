using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Threading;
using System.Text.Json.Serialization;
using System.Text.Json;

// Copied from MS DOCS
public class PortChat
{
    static bool _continue;
    static SerialPort _serialPort;
    
    public static void Main()
    {
        string message;
        StringComparer stringComparer = StringComparer.OrdinalIgnoreCase;
        Thread readThread = new Thread(Read);

        _serialPort = new SerialPort();

        _serialPort.PortName = "COM6";
        _serialPort.BaudRate = 115200;

        _serialPort.ReadTimeout = 500;
        _serialPort.WriteTimeout = 500;

        _serialPort.Open();
        _continue = true;
        readThread.Start();

        Console.WriteLine("Type QUIT to exit");

        while (_continue)
        {
            message = Console.ReadLine();

            if (stringComparer.Equals("quit", message))
            {
                _continue = false;
            }
            else
            {
                // About 800 chars is the limit for one message
                var myOpt = new Request
                {
                    Id = Guid.NewGuid().ToString() + "ASDSDGJDOIJGLRSKFLJGLSE%6666666666666666666675675867867856897896689oziukjmvhbdfgvdrdgfdrfcsrdthjhtresdghgjuft6ztgfdrtzujhgfdjgjjfdklggsdtrgvdrbtfjtfhvgdtrcfedwsadfcdvgbgzftgvrdwssdefrvgtbgoioigfudofiugoixdfugodigjkdflkjfdkjdfjkfdkjlkjjkljkldjkljkljklgjfsdjkldfsjklfsdjkfsdjksdfljksfdjlksdfjklfsdjklfsdjklsfdjklsfdjlkfsdjklfsdjlkfsdlkjfsdjkldsfjlkfsdjlksfdadfcdvgbgzftgvrdwssdefrvgtbgoioigfudofiugoixdfugodigjkdflkjfdkjdfjkfdkjlkjjkljkldjkljkljklgjfsdjkldfsjklfsdjkfsdjksdfljksfdjlksdfjklfsdjklfsdjklsfdjklsfdjlkfsdjklfsdjlkfsdlkjfsdjkldsfjlkfsdjlksfdjlksdfjklsfdjklsfdjkdsfjklfjkljkldjkljklfkjldfsddsfhsdfsfdsdfsdfsfdkhsfdsfdsfdhsfdsfdhkfsdsfdhsdfksfdhfdssdfkhfdssdfhfrwviozf94875t9084fr",
                    Action = "run!",
                    Config = new Config
                    {
                        ValueA = "123",
                        ValueB = "667"
                    }
                };

                Console.WriteLine("Writing " + JsonSerializer.Serialize(myOpt));

                Log("Writing");
                _serialPort.WriteLine(JsonSerializer.Serialize(myOpt));
                Log("Written");
            }
        }

        readThread.Join();
        _serialPort.Close();
    }

    private static void Log(string str)
    {
        Console.WriteLine($"{DateTime.UtcNow.ToString("o")}: {str}");
    }

    public static void Read()
    {
        while (_continue)
        {
            try
            {
                string message = _serialPort.ReadLine();

                Log("Read");
                Console.WriteLine(message);
            }
            catch (TimeoutException) { }
        }
    }

    public class Request
    {
        [JsonPropertyName("id")]
        public string Id { get; set; }

        [JsonPropertyName("action")]
        public string Action { get; set; }

        [JsonPropertyName("config")]
        public Config Config { get; set; }
    }

    public class Config
    {
        [JsonPropertyName("valueA")]
        public string ValueA { get; set; }

        [JsonPropertyName("valueB")]
        public string ValueB { get; set; }
    }
}