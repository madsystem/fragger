using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Fragger
{
    class PipeServer
    {
        public static void Start()
        {
            var task = Task.Factory.StartNew(() =>
            {
                var server = new NamedPipeServerStream("FRAGGER_PIPE");
                server.WaitForConnection();
                StreamReader reader = new StreamReader(server);
                StreamWriter writer = new StreamWriter(server);
                while (true)
                {
                    var line = reader.ReadLine();
                    writer.WriteLine(String.Join("", line.Reverse()));
                    writer.Flush();
                }
            });

        }
    }
}
