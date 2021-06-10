using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Interfaz
{
    public partial class UnsavedChangesAlert : Form
    {
        bool Selected;
        public bool getSaved()
        {
            /*while (!Selected)
            {

            }*/
            return true;
        }
        public UnsavedChangesAlert()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Selected = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Selected = true;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Selected = true;
        }
    }
}
