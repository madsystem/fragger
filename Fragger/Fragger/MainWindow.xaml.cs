using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Fragger
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window , INotifyPropertyChanged
    {
        public string ProcessName { get; set; }
        public string DLLLocation { get; set; }
        PipeServer pipeServer = new PipeServer("FRAGGER_PIPE");
        public event PropertyChangedEventHandler PropertyChanged;

        private Int32 currentFragmentation = -1;
        public int CurrentFragmentation
        {
            get {return currentFragmentation;}
            set {currentFragmentation = value;
                NotifyChange(new PropertyChangedEventArgs("CurrentFragmentation"));}
        }

        private void NotifyChange(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, e);
        }

        public MainWindow()
        {
            this.ProcessName = "notepad++";
            this.DLLLocation = "J:/Fragger/Fragger/Debug/FraggerProbe.dll";
            this.CurrentFragmentation = -1;
            this.pipeServer.PipeDataReceived += OnPipeDataReceived;
            
            InitializeComponent();
            this.DataContext = this;
        }

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            this.pipeServer.Start();
            Injector.Inject(ProcessName, DLLLocation);
        }

        private void OnPipeDataReceived(object sender, PipeServer.PipeData pipeData)
        {
            this.CurrentFragmentation = Int32.Parse(pipeData.PipeDataStr);
        }
    }
}
