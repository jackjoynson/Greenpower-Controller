namespace ControlPanel
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
        protected override void Dispose( bool disposing )
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose( disposing );
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.glgControl1 = new GenLogic.GlgControl();
            this.SuspendLayout();
            // 
            // glgControl1
            // 
            this.glgControl1.DrawingFile = "";
            this.glgControl1.DrawingObject = null;
            this.glgControl1.DrawingURL = "";
            this.glgControl1.HierarchyEnabled = true;
            this.glgControl1.Location = new System.Drawing.Point( 0, 0 );
            this.glgControl1.MinimumSize = new System.Drawing.Size( 5, 5 );
            this.glgControl1.Name = "glgControl1";
            this.glgControl1.SelectEnabled = true;
            this.glgControl1.Size = new System.Drawing.Size( 359, 340 );
            this.glgControl1.TabIndex = 0;
            this.glgControl1.Text = "glgControl1";
            this.glgControl1.TraceEnabled = true;
            this.glgControl1.GlgInput += new GenLogic.GlgInputEventHandler( this.glgControl1_GlgInput );
            this.glgControl1.GlgH += new GenLogic.GlgHEventHandler( this.glgControl1_GlgH );
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF( 6F, 13F );
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size( 384, 364 );
            this.Controls.Add( this.glgControl1 );
            this.Name = "Form1";
            this.Text = "Form1";
            this.Resize += new System.EventHandler( this.Form1_Resize );
            this.ResumeLayout( false );

        }

        #endregion

        private GenLogic.GlgControl glgControl1;
    }
}

