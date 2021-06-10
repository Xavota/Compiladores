using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;
using System.Diagnostics;

using System.Runtime.InteropServices;

using System.IO;

namespace Interfaz
{
    public partial class Form1 : Form
    {
        string searchInitialDirectory = "";

        string fileName = "Untiled.mara";
        bool wasFirstSaved = false;
        bool isSaved = false;

        dynamic compilerDLLInstance;

        public Form1()
        {
            InitializeComponent();

            string exePath = Process.GetCurrentProcess().MainModule.FileName;
            DirectoryInfo x = Directory.GetParent(exePath);
            String fullPath = x.FullName;
            x = Directory.GetParent(fullPath);
            fullPath = x.FullName;
            x = Directory.GetParent(fullPath);
            fullPath = x.FullName;
            x = Directory.GetParent(fullPath);
            fullPath = x.FullName;
            searchInitialDirectory = x.FullName;

            BinaryType bt;
            if (GetBinaryType(exePath, out bt))
            {
                if (bt == BinaryType.SCS_64BIT_BINARY)
                {
                    fullPath += "\\x64";
                }
            }/**/

            if (IsAssemblyDebugBuild(Assembly.GetExecutingAssembly().Location))
            {
                fullPath += "\\Debug";
            }
            else
            {
                fullPath += "\\Release";
            }

            fullPath += "\\Compilador.dll";


            var DLL = Assembly.UnsafeLoadFrom(fullPath);
            var DLLtype = DLL.GetType("Compilador.Manager");
            compilerDLLInstance = Activator.CreateInstance(DLLtype);


            x = Directory.GetParent(searchInitialDirectory);
            searchInitialDirectory = x.FullName;
            searchInitialDirectory += "\\Files";
        }

        public static ushort GetPEArchitecture(string pFilePath)
        {
            ushort architecture = 0;
            try
            {
                using (System.IO.FileStream fStream = new System.IO.FileStream(pFilePath, System.IO.FileMode.Open, System.IO.FileAccess.Read))
                {
                    using (System.IO.BinaryReader bReader = new System.IO.BinaryReader(fStream))
                    {
                        if (bReader.ReadUInt16() == 23117) //check the MZ signature
                        {
                            fStream.Seek(0x3A, System.IO.SeekOrigin.Current); //seek to e_lfanew.
                            fStream.Seek(bReader.ReadUInt32(), System.IO.SeekOrigin.Begin); //seek to the start of the NT header.
                            if (bReader.ReadUInt32() == 17744) //check the PE\0\0 signature.
                            {
                                fStream.Seek(20, System.IO.SeekOrigin.Current); //seek past the file header,
                                architecture = bReader.ReadUInt16(); //read the magic number of the optional header.
                            }
                        }
                    }
                }
            }
            catch (Exception) { /* TODO: Any exception handling you want to do, personally I just take 0 as a sign of failure */}
            //if architecture returns 0, there has been an error.
            return architecture;
        }

        private bool IsAssemblyDebugBuild(string filepath)
        {
            return IsAssemblyDebugBuild(Assembly.LoadFile(Path.GetFullPath(filepath)));
        }

        private bool IsAssemblyDebugBuild(Assembly assembly)
        {
            foreach (var attribute in assembly.GetCustomAttributes(false)) //obtiene los atributos del assembly
            {
                var debuggableAttribute = attribute as DebuggableAttribute; // trata de castear a debug
                if (debuggableAttribute != null) // Si el casteo fue exitoso
                {
                    return debuggableAttribute.IsJITTrackingEnabled; // Regresa si es de tipo debug o release
                }
            }
            return false; // si no se pudo regresa falso
        }

        public enum BinaryType : uint
        {
            SCS_32BIT_BINARY = 0,   // A 32-bit Windows-based application
            SCS_64BIT_BINARY = 6,   // A 64-bit Windows-based application.
            SCS_DOS_BINARY = 1,     // An MS-DOS � based application
            SCS_OS216_BINARY = 5,   // A 16-bit OS/2-based application
            SCS_PIF_BINARY = 3,     // A PIF file that executes an MS-DOS � based application
            SCS_POSIX_BINARY = 4,   // A POSIX � based application
            SCS_WOW_BINARY = 2      // A 16-bit Windows-based application
        }

        [DllImport("kernel32.dll")]
        static extern bool GetBinaryType(string lpApplicationName, out BinaryType lpBinaryType);


        private void buildButton(object sender, EventArgs e)
        {
        }

        private void newButton()
        {
            if (checkUnsaved())
            {
                fileName = "Untiled.mara";
                wasFirstSaved = false;
                isSaved = false;
                txt_code.Text = "";

                data_tokens.Rows.Clear();
                txt_console.Text = "";

            }
        }

        private void openButton()
        {
            if (checkUnsaved())
            {
                var fileContent = string.Empty;
                var filePath = string.Empty;

                using (OpenFileDialog openFileDialog = new OpenFileDialog())
                {
                    openFileDialog.InitialDirectory = searchInitialDirectory;
                    openFileDialog.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
                    openFileDialog.FilterIndex = 2;
                    openFileDialog.RestoreDirectory = true;

                    if (openFileDialog.ShowDialog() == DialogResult.OK)
                    {
                        //Get the path of specified file
                        filePath = openFileDialog.FileName;

                        //Read the contents of the file into a stream
                        var fileStream = openFileDialog.OpenFile();

                        using (StreamReader reader = new StreamReader(fileStream))
                        {
                            fileContent = reader.ReadToEnd();
                        }
                    }

                    int lines = 1;
                    for (int i = 0; i < fileContent.Length; i++)
                    {
                        if (fileContent[i] == '\n')
                            lines++;
                    }

                    string[] a = new string[lines];

                    int line = 0;

                    for (int i = 0; i < fileContent.Length; i++)
                    {
                        if (fileContent[i] == '\n')
                            line++;
                        else
                        {
                            a[line] += fileContent[i];
                        }
                    }


                    txt_code.Lines = a;
                    fileName = openFileDialog.FileName;
                    wasFirstSaved = true;
                    isSaved = true;

                    data_tokens.Rows.Clear();
                    txt_console.Text = "";

                }
            }

        }

        private void saveButton()
        {
            trySave();
        }

        private void saveAsButton()
        {
            saveAsFile();
        }

        private void exitButton()
        {
            if (checkUnsaved())
            {
                this.Close();
            }
        }

        private void trySave()
        {
            if (!wasFirstSaved)
            {
                saveAsFile();
            }
            else
            {
                saveFile(fileName, txt_code.Text);
            }
            isSaved = true;
        }

        private void saveAsFile()
        {
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            saveFileDialog1.InitialDirectory = searchInitialDirectory;
            saveFileDialog1.Title = "Save As";
            //saveFileDialog1.CheckFileExists = true;
            saveFileDialog1.CheckPathExists = true;
            saveFileDialog1.DefaultExt = "mara";
            saveFileDialog1.Filter = "Mara files (*.mara)|*.mara|All files (*.*)|*.*";
            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                saveFile(saveFileDialog1.FileName, txt_code.Text);

                fileName = saveFileDialog1.FileName;
                wasFirstSaved = true;
                isSaved = true;

            }
            else
            {
                MessageBox.Show("Not saved");
            }
        }

        private void saveFile(string _fileName, string _text)
        {
            using (FileStream fs = File.Create(_fileName))
            {
                byte[] info = new UTF8Encoding(true).GetBytes(_text);
                // Add some information to the file.
                fs.Write(info, 0, info.Length);
            }
        }

        private bool checkUnsaved()
        {
            if (!isSaved)
            {
                DialogResult result = MessageBox.Show("Do you want to save the unsaved changes on file " + fileName, "Warning", MessageBoxButtons.YesNoCancel);
                if (result == DialogResult.Yes) 
                {
                    trySave();
                }
                else if (result == DialogResult.Cancel)
                {
                    return false;
                }
            }
            return true;
        }


        private void New_Click(object sender, EventArgs e)
        {
            newButton();
        }

        private void Open_Click(object sender, EventArgs e)
        {
            openButton();
        }

        private void Save_Click(object sender, EventArgs e)
        {
            saveButton();
        }

        private void SaveAs_Click(object sender, EventArgs e)
        {
            saveAsButton();
        }

        private void Exit_Click(object sender, EventArgs e)
        {
            exitButton();
        }

        private void txt_code_TextChanged_1(object sender, EventArgs e)
        {
            isSaved = false;
        }

        private void toolStripMenuItem8_Click(object sender, EventArgs e)
        {

            data_tokens.Rows.Clear();

            String[] result = compilerDLLInstance.Compilar(txt_code.Text);

            List<string> outputStrings = new List<string>();

            int flag = 0;

            int readingTokens = 1;
            int readingErrors = 2;
            int readingComments = 3;

            for (int i = 0; i < result.Length; i++)
            {
                if (result[i] == "@Tokens")
                {
                    flag = readingTokens;
                    continue;
                }                
                else if(result[i] == "@Errors")
                {
                    flag = readingErrors;
                    continue;
                }
                else if (result[i] == "@Comments")
                {
                    flag = readingComments;
                    if (outputStrings.Count > 0)
                        outputStrings.Add("");
                    continue;
                }

                if (flag == readingTokens)
                {
                    string line = "";
                    string lexeme = "";
                    string type = "";

                    int tokenFlag = 0;

                    int readingLine = 0;
                    int readingLexeme = 1;
                    int readingType = 2;

                    for (int j = 0; j < result[i].Length; j++)
                    {
                        if (result[i][j] == '\n')
                        {
                            tokenFlag++;
                        }

                        if (tokenFlag == readingLine)
                        {
                            line += result[i][j];
                        }
                        else if (tokenFlag == readingLexeme)
                        {
                            lexeme += result[i][j];
                        }
                        else if (tokenFlag == readingType)
                        {
                            type += result[i][j];
                        }
                    }
                    data_tokens.Rows.Add(line, lexeme, type);
                }
                else if (flag == readingErrors)
                {
                    outputStrings.Add("ERROR: " + result[i]);
                }
                else if (flag == readingComments)
                {
                    outputStrings.Add(result[i]);
                }
            }

            txt_console.Lines = outputStrings.ToArray();
        }
    }
}
