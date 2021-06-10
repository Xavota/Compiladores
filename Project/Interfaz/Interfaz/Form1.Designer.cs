
namespace Interfaz
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip2 = new System.Windows.Forms.MenuStrip();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.New = new System.Windows.Forms.ToolStripMenuItem();
            this.Open = new System.Windows.Forms.ToolStripMenuItem();
            this.Save = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem6 = new System.Windows.Forms.ToolStripSeparator();
            this.Exit = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem8 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem5 = new System.Windows.Forms.ToolStripMenuItem();
            this.txt_code = new System.Windows.Forms.TextBox();
            this.txt_console = new System.Windows.Forms.TextBox();
            this.data_things = new System.Windows.Forms.DataGridView();
            this.Line = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._Name = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Category = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Length = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Type = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Function = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.data_tokens = new System.Windows.Forms.DataGridView();
            this.Token_Line = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Lexeme = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Token_Type = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.menuStrip2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.data_things)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.data_tokens)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip2
            // 
            this.menuStrip2.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem2,
            this.toolStripMenuItem3,
            this.toolStripMenuItem4,
            this.toolStripMenuItem5});
            this.menuStrip2.Location = new System.Drawing.Point(0, 0);
            this.menuStrip2.Name = "menuStrip2";
            this.menuStrip2.Size = new System.Drawing.Size(1213, 30);
            this.menuStrip2.TabIndex = 0;
            this.menuStrip2.Text = "menuStrip2";
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.New,
            this.Open,
            this.Save,
            this.SaveAs,
            this.toolStripMenuItem6,
            this.Exit});
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(46, 26);
            this.toolStripMenuItem2.Text = "File";
            // 
            // New
            // 
            this.New.Name = "New";
            this.New.Size = new System.Drawing.Size(139, 26);
            this.New.Text = "New";
            this.New.Click += new System.EventHandler(this.New_Click);
            // 
            // Open
            // 
            this.Open.Name = "Open";
            this.Open.Size = new System.Drawing.Size(139, 26);
            this.Open.Text = "Open";
            this.Open.Click += new System.EventHandler(this.Open_Click);
            // 
            // Save
            // 
            this.Save.Name = "Save";
            this.Save.Size = new System.Drawing.Size(139, 26);
            this.Save.Text = "Save";
            this.Save.Click += new System.EventHandler(this.Save_Click);
            // 
            // SaveAs
            // 
            this.SaveAs.Name = "SaveAs";
            this.SaveAs.Size = new System.Drawing.Size(139, 26);
            this.SaveAs.Text = "SaveAs";
            this.SaveAs.Click += new System.EventHandler(this.SaveAs_Click);
            // 
            // toolStripMenuItem6
            // 
            this.toolStripMenuItem6.Name = "toolStripMenuItem6";
            this.toolStripMenuItem6.Size = new System.Drawing.Size(136, 6);
            // 
            // Exit
            // 
            this.Exit.Name = "Exit";
            this.Exit.Size = new System.Drawing.Size(139, 26);
            this.Exit.Text = "Exit";
            this.Exit.Click += new System.EventHandler(this.Exit_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem8});
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(57, 26);
            this.toolStripMenuItem3.Text = "Build";
            // 
            // toolStripMenuItem8
            // 
            this.toolStripMenuItem8.Name = "toolStripMenuItem8";
            this.toolStripMenuItem8.Size = new System.Drawing.Size(126, 26);
            this.toolStripMenuItem8.Text = "Build";
            this.toolStripMenuItem8.Click += new System.EventHandler(this.toolStripMenuItem8_Click);
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(75, 26);
            this.toolStripMenuItem4.Text = "Options";
            // 
            // toolStripMenuItem5
            // 
            this.toolStripMenuItem5.Name = "toolStripMenuItem5";
            this.toolStripMenuItem5.Size = new System.Drawing.Size(55, 26);
            this.toolStripMenuItem5.Text = "Help";
            // 
            // txt_code
            // 
            this.txt_code.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txt_code.Location = new System.Drawing.Point(8, 34);
            this.txt_code.Multiline = true;
            this.txt_code.Name = "txt_code";
            this.txt_code.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txt_code.Size = new System.Drawing.Size(889, 473);
            this.txt_code.TabIndex = 1;
            this.txt_code.TextChanged += new System.EventHandler(this.txt_code_TextChanged_1);
            // 
            // txt_console
            // 
            this.txt_console.Location = new System.Drawing.Point(8, 513);
            this.txt_console.Multiline = true;
            this.txt_console.Name = "txt_console";
            this.txt_console.ReadOnly = true;
            this.txt_console.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txt_console.Size = new System.Drawing.Size(1193, 135);
            this.txt_console.TabIndex = 2;
            // 
            // data_things
            // 
            this.data_things.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.data_things.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Line,
            this._Name,
            this.Category,
            this.Length,
            this.Type,
            this.Function});
            this.data_things.Location = new System.Drawing.Point(8, 654);
            this.data_things.Name = "data_things";
            this.data_things.RowHeadersWidth = 51;
            this.data_things.RowTemplate.Height = 24;
            this.data_things.Size = new System.Drawing.Size(1193, 137);
            this.data_things.TabIndex = 3;
            // 
            // Line
            // 
            this.Line.HeaderText = "Line";
            this.Line.MinimumWidth = 6;
            this.Line.Name = "Line";
            this.Line.Width = 40;
            // 
            // _Name
            // 
            this._Name.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this._Name.HeaderText = "Name";
            this._Name.MinimumWidth = 6;
            this._Name.Name = "_Name";
            // 
            // Category
            // 
            this.Category.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Category.HeaderText = "Category";
            this.Category.MinimumWidth = 6;
            this.Category.Name = "Category";
            // 
            // Length
            // 
            this.Length.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Length.HeaderText = "Length";
            this.Length.MinimumWidth = 6;
            this.Length.Name = "Length";
            // 
            // Type
            // 
            this.Type.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Type.HeaderText = "Data type";
            this.Type.MinimumWidth = 6;
            this.Type.Name = "Type";
            // 
            // Function
            // 
            this.Function.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Function.HeaderText = "Function";
            this.Function.MinimumWidth = 6;
            this.Function.Name = "Function";
            // 
            // data_tokens
            // 
            this.data_tokens.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.data_tokens.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Token_Line,
            this.Lexeme,
            this.Token_Type});
            this.data_tokens.Location = new System.Drawing.Point(904, 32);
            this.data_tokens.Name = "data_tokens";
            this.data_tokens.RowHeadersWidth = 51;
            this.data_tokens.RowTemplate.Height = 24;
            this.data_tokens.Size = new System.Drawing.Size(297, 475);
            this.data_tokens.TabIndex = 4;
            // 
            // Token_Line
            // 
            this.Token_Line.HeaderText = "Line";
            this.Token_Line.MinimumWidth = 6;
            this.Token_Line.Name = "Token_Line";
            this.Token_Line.Width = 40;
            // 
            // Lexeme
            // 
            this.Lexeme.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Lexeme.HeaderText = "Lexeme";
            this.Lexeme.MinimumWidth = 6;
            this.Lexeme.Name = "Lexeme";
            // 
            // Token_Type
            // 
            this.Token_Type.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Token_Type.HeaderText = "Type";
            this.Token_Type.MinimumWidth = 6;
            this.Token_Type.Name = "Token_Type";
            // 
            // Form1
            // 
            this.ClientSize = new System.Drawing.Size(1213, 803);
            this.Controls.Add(this.data_tokens);
            this.Controls.Add(this.data_things);
            this.Controls.Add(this.txt_console);
            this.Controls.Add(this.txt_code);
            this.Controls.Add(this.menuStrip2);
            this.MainMenuStrip = this.menuStrip2;
            this.Name = "Form1";
            this.menuStrip2.ResumeLayout(false);
            this.menuStrip2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.data_things)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.data_tokens)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem buildToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem buildSolutionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem1;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exiToolStripMenuItem;
        private System.Windows.Forms.DataGridView data_;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Nombre;
        private System.Windows.Forms.DataGridViewTextBoxColumn Categoria;
        private System.Windows.Forms.DataGridViewTextBoxColumn Longitud;
        private System.Windows.Forms.DataGridViewTextBoxColumn TipoDeDato;
        private System.Windows.Forms.DataGridViewTextBoxColumn Funcion;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Linea;
        private System.Windows.Forms.DataGridViewTextBoxColumn Lexema;
        private System.Windows.Forms.DataGridViewTextBoxColumn Tipo;
        private System.Windows.Forms.MenuStrip menuStrip2;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem New;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem5;
        private System.Windows.Forms.ToolStripMenuItem Open;
        private System.Windows.Forms.ToolStripMenuItem Save;
        private System.Windows.Forms.ToolStripMenuItem SaveAs;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem6;
        private System.Windows.Forms.ToolStripMenuItem Exit;
        private System.Windows.Forms.TextBox txt_code;
        private System.Windows.Forms.TextBox txt_console;
        private System.Windows.Forms.DataGridView data_things;
        private System.Windows.Forms.DataGridView data_tokens;
        private System.Windows.Forms.DataGridViewTextBoxColumn Token_Line;
        private System.Windows.Forms.DataGridViewTextBoxColumn Lexeme;
        private System.Windows.Forms.DataGridViewTextBoxColumn Token_Type;
        private System.Windows.Forms.DataGridViewTextBoxColumn Line;
        private System.Windows.Forms.DataGridViewTextBoxColumn _Name;
        private System.Windows.Forms.DataGridViewTextBoxColumn Category;
        private System.Windows.Forms.DataGridViewTextBoxColumn Length;
        private System.Windows.Forms.DataGridViewTextBoxColumn Type;
        private System.Windows.Forms.DataGridViewTextBoxColumn Function;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem8;
    }
}

