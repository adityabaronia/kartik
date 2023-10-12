namespace C2server
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
            this.launch_btn = new System.Windows.Forms.Button();
            this.cmdbox = new System.Windows.Forms.TextBox();
            this.lbloutput = new System.Windows.Forms.Label();
            this.cmdtype_dropdown = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // launch_btn
            // 
            this.launch_btn.Location = new System.Drawing.Point(428, 159);
            this.launch_btn.Name = "launch_btn";
            this.launch_btn.Size = new System.Drawing.Size(75, 23);
            this.launch_btn.TabIndex = 0;
            this.launch_btn.Text = "Launch";
            this.launch_btn.UseVisualStyleBackColor = true;
            this.launch_btn.Click += new System.EventHandler(this.launch_btn_Click);
            // 
            // cmdbox
            // 
            this.cmdbox.Location = new System.Drawing.Point(106, 75);
            this.cmdbox.Name = "cmdbox";
            this.cmdbox.Size = new System.Drawing.Size(586, 20);
            this.cmdbox.TabIndex = 2;
            this.cmdbox.TextChanged += new System.EventHandler(this.cmdbox_TextChanged);
            // 
            // lbloutput
            // 
            this.lbloutput.AutoSize = true;
            this.lbloutput.Location = new System.Drawing.Point(390, 256);
            this.lbloutput.Name = "lbloutput";
            this.lbloutput.Size = new System.Drawing.Size(37, 13);
            this.lbloutput.TabIndex = 3;
            this.lbloutput.Text = "output";
            this.lbloutput.Click += new System.EventHandler(this.lbloutput_Click);
            // 
            // cmdtype_dropdown
            // 
            this.cmdtype_dropdown.FormattingEnabled = true;
            this.cmdtype_dropdown.Location = new System.Drawing.Point(106, 160);
            this.cmdtype_dropdown.Name = "cmdtype_dropdown";
            this.cmdtype_dropdown.Size = new System.Drawing.Size(121, 21);
            this.cmdtype_dropdown.TabIndex = 4;
            this.cmdtype_dropdown.Text = "cmdtype";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.cmdtype_dropdown);
            this.Controls.Add(this.lbloutput);
            this.Controls.Add(this.cmdbox);
            this.Controls.Add(this.launch_btn);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button launch_btn;
        private System.Windows.Forms.TextBox cmdbox;
        private System.Windows.Forms.Label lbloutput;
        private System.Windows.Forms.ComboBox cmdtype_dropdown;
    }
}

