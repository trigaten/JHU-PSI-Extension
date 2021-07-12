using System;
using Microsoft.Psi;
using Microsoft.Psi.Interop.Format;
using Microsoft.Psi.Interop.Transport;
using System.Linq;
namespace SoundDataArduino
{
    class Program
    {
        static void Main(string[] args)
        {
            var p = Pipeline.Create();
            var soundDataSource = new NetMQSource<dynamic>(
                    p,
                    "sound",
                    "tcp://127.0.0.1:30001",
                    MessagePackFormat.Instance);

            var soundData = soundDataSource.Select(rects => rects).Out;

            soundData.Do(e => Console.WriteLine(e));

            p.RunAsync();

            Console.WriteLine("Press Enter key to quit...");
            Console.ReadLine();
        }

    }
}
