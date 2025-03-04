
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;

namespace ConsoleApp1
{
    class Hub
    {
#if DEBUG
        public const string suffix = "d";
#else
        public const string suffix = "";
#endif

        public const string dllName = "hub" + suffix;

        [DllImport(dllName, EntryPoint = "helloWorld", CallingConvention = CallingConvention.Cdecl)]
        public static extern void helloWorld();

        [DllImport(dllName, EntryPoint = "freeOutputSensor", CallingConvention = CallingConvention.Cdecl)]
        public static extern void freeOutputSensor(IntPtr outputSensor);

    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello, world! from C#");

            Hub.helloWorld();
            //Hub.freeOutputSensor(IntPtr.Zero);
            Console.ReadLine();
        }
    }
}
