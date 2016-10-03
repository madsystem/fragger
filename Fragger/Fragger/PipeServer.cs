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
        public event EventHandler<PipeData> PipeDataReceived;
        private string pipeName;

        public PipeServer(string pipeName)
        {
            this.pipeName = pipeName;
        }

        public void Start()
        {
            var task = Task.Factory.StartNew(() =>
            {
                var server = new NamedPipeServerStream(this.pipeName);
                server.WaitForConnection();
                StreamReader reader = new StreamReader(server);
                StreamWriter writer = new StreamWriter(server);
                while (true)
                {
                    var line = reader.ReadLine();

                    PipeData pipeData = new PipeData();
                    pipeData.PipeDataStr = line;
                    this.OnPipeDataReceived(pipeData);
                    
                    writer.Flush();
                }
            });

        }

        protected virtual void OnPipeDataReceived(PipeData e)
        {
            PipeDataReceived?.Invoke(this, e);
        }

        public class PipeData : EventArgs
        {
            public string PipeDataStr { get; set; }
            
        }
    }
}
