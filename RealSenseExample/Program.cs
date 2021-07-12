using System;
using Microsoft.Psi;
using Microsoft.Psi.RealSense.Windows;
using System.Text;
using Microsoft.Psi.Imaging;
using Microsoft.Psi.RealSense;


namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            var p = Pipeline.Create();
            RealSenseSensor r = new(p);
            const int widthOutput = 160;
            const int heightOutput = 90;

            StringBuilder sb = new StringBuilder();
            r.ColorImage.Resize(widthOutput, heightOutput).Do(color =>
            {
                var bitmap = color.Resource.ToBitmap();

                // render color frame as "ASCII art"
                sb.Clear();
                for (int y = 0; y < heightOutput; y += 2)
                {
                    for (int x = 0; x < widthOutput; x++)
                    {
                        var p = bitmap.GetPixel(x, y);
                        sb.Append(" .:-=+*#%@"[(int)((p.R + p.G + p.B) / 76.5)]);
                    }

                    sb.Append(Environment.NewLine);
                }

                Console.SetCursorPosition(0, 0);
                Console.WriteLine(sb.ToString());
            });

            Console.BackgroundColor = ConsoleColor.Black;
            Console.ForegroundColor = ConsoleColor.White;
            Console.Clear();
            p.RunAsync();
            Console.ReadLine(); // press Enter to end


            //var range = Generators.Range(p, 1, 5, TimeSpan.FromSeconds(0));

            //var latencyInducer = new LatencyInducer<int>(p, TimeSpan.FromSeconds(5));

            //range.PipeTo(latencyInducer.ItemIn);

            //latencyInducer.Out.Do(b => Console.WriteLine(b));
        }
    }
}
