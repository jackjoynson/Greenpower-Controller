using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using GenLogic;


namespace ControlPanel
{
    public partial class Form1 : Form
    {
		/* Initial parameters for a dial */
		double LowRange = 0.0;
		double HighRange = 40.0;
		double InitValue = 20.0;

        //MY ADDED VARIABLES
        double currentValue = 0.0; //THIS NEEDS TO COME FROM ARDUINO

        public Form1()
        {
            InitializeComponent();

            // Set the drawing name.
            String drawing_file = "C:\\Users\\Judy\\Documents\\GREENPOWER CONTROLLER\\TEST.g";
            glgControl1.DrawingFile = drawing_file;

            ResizeGLGControl();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // HCallback is invoked after the drawing is loaded, before object hirarchy 
        // is set up, but before the drawing is dispalyed for the first time.
        // It may be used to initialize the drawing before it is displayed.
        ///////////////////////////////////////////////////////////////////////////////
        private void glgControl1_GlgH( object sender, GenLogic.GlgEventArgs e )
        {
            // Set initial values for a GLG widget.

			// Disable InputHandler for a dial control so that the dial acts
			// only as an output device and doesn't respond to user input.
			e.viewport.SetDResource( "Dial/HandlerDisabled", 1.0 );

			// Set High and Low range resources for both Slider and Dial
			e.viewport.SetDResource( "Dial/Low", LowRange );
   
			e.viewport.SetDResource( "Dial/High", HighRange );
   
			// Set initial values of a dial and a slider
			e.viewport.SetDResource( "Dial/Value", InitValue );

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Handle input events.
        ///////////////////////////////////////////////////////////////////////////////
        private void glgControl1_GlgInput( object sender, GenLogic.GlgInputEventArgs e )
        {
            string
                origin,
                format,
                action;

            origin = e.message_obj.GetSResource( "Origin" );
            format = e.message_obj.GetSResource( "Format" );
            action = e.message_obj.GetSResource( "Action" );

      		// Process events from a button.
            if (format.Equals( "Button" ) && action.Equals( "Activate" ))
			{
                if (origin.Equals("UpdateButton"))
                {
                    e.viewport.SetDResource("Dial/Value", currentValue);
                }
                else if (origin.Equals("QuitButton"))
                {
                    Application.Exit();    // Exit
                }
			}

			e.viewport.UpdateGlg();
        }

        /////////////////////////////////////////////////////////////////////////////
        private void Form1_Resize( object sender, EventArgs e )
        {
            ResizeGLGControl();
        }

        //////////////////////////////////////////////////////////////////////////
        // Set width and height of the GLG control.
        //////////////////////////////////////////////////////////////////////
        private void ResizeGLGControl()
        {
            // Resize GLG control according to the form's client size.
            glgControl1.Size = ClientSize;
        }
    }
}
