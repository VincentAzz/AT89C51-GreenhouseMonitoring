using Microsoft.Win32;

namespace TempAutoControl
{
    public partial class Form_Main : Form
    {
        public Form_Main()
        {
            InitializeComponent();
        }

        private void getSerialPorts()
        {     
            RegistryKey keyComm = Registry.LocalMachine.OpenSubKey("Hardware\\DeviceMap\\SerialComm");
            String[] comNames = keyComm.GetValueNames();
            if(comNames.Length!=0)
            {
                foreach(String s in comNames)
                {
                    comboBox1_SerialPorts.Items.Add(keyComm.GetValue(s));
                }
                comboBox1_SerialPorts.SelectedIndex = 0;
            }
        }

        private void Form_Main_QueryAccessibilityHelp(object sender, QueryAccessibilityHelpEventArgs e)
        {

        }

        private void Form_Main_Load(object sender, EventArgs e)
        {
            comboBox1_SerialPorts.Items.Clear();
            getSerialPorts();
        }

        private void Form_Main_FormClosed(object sender, FormClosedEventArgs e)
        {

        }

        private void Form_Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if  (
                MessageBox.Show(
                    "你确定关闭系统吗？",
                    "确定",
                    MessageBoxButtons.YesNo,
                    MessageBoxIcon.Exclamation,
                    MessageBoxDefaultButton.Button2
                    )
                == DialogResult.No
                ){
                e.Cancel = true;
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {

        }
    }
}