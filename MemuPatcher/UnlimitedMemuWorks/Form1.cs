using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UnlimitedMemuWorks
{

    public partial class Form1 : Form
    {

        //Those are the strings we have to replace in RAM to foil DW's shitty system file lookup
        readonly String microvirtString = "microvirt";
        //We can't just replace all "memu" strings obviously, so we cherrypick unlike with microvirt
        readonly String memuString = "memu";
        readonly String[] memuStrings = { "memuime", "memud", "memuguest.ko", "memusf.ko" }; 

        public Form1()
        {
            InitializeComponent();
            CheckProcessAndUpdateGUI();

        }

        private Boolean CheckForMemu()
        {
            Process[] procs = Process.GetProcessesByName("MemuHeadless");
            return (procs.Length > 0);  //If there's a process, we're good to go
        }

        private void hackMemu()
        {

            if (CheckForMemu())
            {
                Process process = Process.GetProcessesByName("MemuHeadless")[0];

                //Time to hack this shit
                AppendTextBox("Opening MemuHeadless process for memory editing.");
                AppendTextBox("Please don't touch the emulator while patching is in progress.\n");
                SetHackingStatus(true);

                //Instantiate a memory scanner
                ProcessMemoryScanner scan = new ProcessMemoryScanner(process);

                //Go through our strings and replace them with the versions specified by the user
                AppendTextBox("---------Replacing microvirt strings...");
                EncodeStringAndReplace(scan, microvirtString, microvirt_textBox.Text, Encoding.Unicode);
                EncodeStringAndReplace(scan, microvirtString, microvirt_textBox.Text, Encoding.UTF8);

                AppendTextBox("---------Replacing memu strings... This might take a while.");
                foreach (String s in memuStrings)
                {
                    String replacement = s.Replace(memuString, memu_textBox.Text);
                    EncodeStringAndReplace(scan, s, replacement, Encoding.Unicode);
                    EncodeStringAndReplace(scan, s, replacement, Encoding.UTF8);
                }

                AppendTextBox("All done! You can start FGO now.");
                SetHackingStatus(false);

            }

            return;
        }

        private void EncodeStringAndReplace(ProcessMemoryScanner scan, string originalString, String replacementString, Encoding encoding)
        {
            byte[] original = encoding.GetBytes(originalString);
            byte[] replacement = encoding.GetBytes(replacementString);
            AppendTextBox(originalString + " -> " + replacementString + " ("+encoding.ToString()+")");
            int result = scan.ReplaceInRAM(original, replacement);
            AppendTextBox("Replaced " + result + " instances.");
        } 

        private void CheckProcessAndUpdateGUI()
        {
            if (CheckForMemu())
            {
                //change gui to indicate memu is present and we can H4CK
                button1.Enabled = true;
                label4.ForeColor = Color.Green;
                label4.Text = "DETECTED";
            }
            else
            {
                button1.Enabled = false;
                label4.ForeColor = Color.Red;
                label4.Text = "NOT DETECTED";
            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            //Start the "hacking" in a separate thread to keep the GUI alive
            richTextBox1.Clear();
            new Thread(hackMemu).Start();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            CheckProcessAndUpdateGUI();
        }


        public void AppendTextBox(string value)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<string>(AppendTextBox), new object[] { value });
                return;
            }
            richTextBox1.Text += value+"\n";
            //Autoscroll
            richTextBox1.SelectionStart = richTextBox1.TextLength;
            richTextBox1.ScrollToCaret();
        }

        public void SetHackingStatus(Boolean isHacking)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<Boolean>(SetHackingStatus), new object[] {isHacking});
                return;
            }
            
            if (isHacking)
            {
                button1.Enabled = false;
                button2.Enabled = false;
                pictureBox1.Visible = true;
                label4.ForeColor = Color.Blue;
                label4.Text = "CODE CASTING";
            }
            else
            {
                button1.Enabled = true;
                button2.Enabled = false;
                pictureBox1.Visible = false;
                label4.ForeColor = Color.Green;
                label4.Text = "PATCHED";
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            var form = new AboutForm();
            form.Show(this); // if you need non-modal window
        }
    }


}
