import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

public class Fractalizer extends JPanel {

	private static Fractalizer	mainPanel;

	public static void main(String[] args) {
		JFrame window = new JFrame("Fractalizer");
		mainPanel = new Fractalizer();
		window.setContentPane(mainPanel);
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		window.pack();
		window.setResizable(false);
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		window.setLocation((screen.width - window.getWidth())/2, (screen.height - window.getHeight())/2);
		window.setVisible(true);
	}

	private static final int
				WINDOW_WIDTH	= 1200,
				WINDOW_HEIGHT	= (WINDOW_WIDTH + 140) / 2,
				TABLE_SIZE	= (WINDOW_WIDTH - 120) / 2,
				MANDEL_START_X	= 40,
				MANDEL_START_Y	= 40,
				JULIA_START_X	= TABLE_SIZE + 80,
				JULIA_START_Y	= 40;

	private DrawingTable	Mandel, Julia;

	private Point			juliaC 			= new Point(0, 0);
	private JLabel			statusLabel		= new JLabel("C = 0+0i", JLabel.CENTER);
	private JToggleButton	zoomMandel		= new JToggleButton("Zoom into the Mandelbrot set");

	private Color[]			colorPalete		= new Color[1024];




	public Fractalizer() {
		setPreferredSize(new Dimension(WINDOW_WIDTH, WINDOW_HEIGHT));
		setLayout(null);

		Mandel	= new DrawingTable(){
			public void paintComponent(Graphics g){
				super.paintComponent(g);
				//draw Mandelbrot set
				int i, j, n;
				Point z, c;
				for(i = 0; i < TABLE_SIZE; i++){
					for(j = 0; j < TABLE_SIZE; j++){

						n = 1023;
						c = Mandel.toComplex(i, j);
						z = new Point(0, 0);
						while (n > 10 && z.abs() < 100) {
							z.f_c(c);
							n -= 10;
						}
						g.setColor(colorPalete[n]);
						g.drawLine(i, j, i, j);
					}
				}
				g.setColor(Color.GREEN);
				c = Mandel.toInteger(juliaC);
				g.drawLine((int)c.x, (int)c.y, (int)c.x, (int)c.y);
			}
			public void  mousePressed(MouseEvent evt) {
				super.mousePressed(evt);
				if(!isZoomActivated){
					juliaC = Mandel.toComplex(new Point(evt.getX(), evt.getY()));
					//Julia.repaint();repaint();
					mainPanel.repaint();
				}
			}
		};
		add(Mandel);
		Mandel.setBounds(MANDEL_START_X, MANDEL_START_Y, TABLE_SIZE, TABLE_SIZE);


		Julia	= new DrawingTable(){
			public void paintComponent(Graphics g){
				super.paintComponent(g);
				//draw Julia set
				int i, j, n;
				Point z, c;
				for(i = 0; i < TABLE_SIZE; i++){
					for(j = 0; j < TABLE_SIZE; j++){

						n = 1023;
						z = Julia.toComplex(i, j);
						while (n > 10 && z.abs() < 100) {
							z.f_c(juliaC);
							n -= 10;
						}
						g.setColor(colorPalete[n]);
						g.drawLine(i, j, i, j);

					}
				}

				//draw post critical set
				g.setColor(Color.GREEN);
				z = new Point(0, 0);
				for(i = 0; i < 1000000; i++){
					z.f_c(juliaC);
					c = Julia.toInteger(z);
					g.drawLine((int)c.x, (int)c.y, (int)c.x, (int)c.y);
				}
			}
		};
		add(Julia);
		Julia.setBounds(JULIA_START_X, JULIA_START_Y, TABLE_SIZE, TABLE_SIZE);

		zoomMandel.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ev) {
				Mandel.isZoomActivated = !(ev.getStateChange()==ItemEvent.SELECTED);
			}
		});

		add(zoomMandel);
//		zoomMandel.setBounds(WINDOW_WIDTH/4, WINDOW_HEIGHT - 80, WINDOW_WIDTH/2, 20);
		zoomMandel.setBounds(MANDEL_START_X, WINDOW_HEIGHT - 60, TABLE_SIZE, 20);

		add(statusLabel);
//		statusLabel.setBounds(40, WINDOW_HEIGHT - 40, WINDOW_WIDTH - 80, 20);
		statusLabel.setBounds(JULIA_START_X, WINDOW_HEIGHT - 60, TABLE_SIZE, 20);

		int[][] c = new int[][]{
			{0, 0, 0},
			{255, 0, 0},
			{135, 206, 235},
			{0, 0, 255},
			{255, 255, 255}
		};
		int color_points = 5;

		for(int k = 0; k < color_points - 1; k++){
			double t;
			for(int i = 0; i < 255; i++){
				t = (float)i / 255.0;
				colorPalete[i + 256*k] = new Color((int)(c[k][0]*(1 - t) + c[k+1][0]*t), (int)(c[k][1]*(1 - t) + c[k+1][1]*t), (int)(c[k][2]*(1 - t) + c[k+1][2]*t));
			};
		};



		repaint();
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		statusLabel.setText("c = " + juliaC.x + "+" + juliaC.y + "i");

		//draw the border
		g.setColor(Color.BLACK);
		g.drawRect(MANDEL_START_X-1, MANDEL_START_Y-1, TABLE_SIZE+1, TABLE_SIZE+1);
		g.drawRect(JULIA_START_X-1, JULIA_START_Y-1, TABLE_SIZE+1, TABLE_SIZE+1);
	}



	private class DrawingTable extends JPanel implements MouseListener{
		Point topLeft;
		double size;
		boolean isZoomActivated = false;

		DrawingTable(){
			setPreferredSize(new Dimension(TABLE_SIZE, TABLE_SIZE));
			topLeft = new Point(-3, -3);
			size = 6;
			addMouseListener(this);
		}

		public Point toComplex(double x, double y){
			return new Point(x*size/TABLE_SIZE+topLeft.x, y*size/TABLE_SIZE+topLeft.y);
		}

		public Point toComplex(Point p){ return toComplex(p.x, p.y); }

		public Point toInteger(double x, double y){
			return new Point((int)((x - topLeft.x)/size * TABLE_SIZE), (int)((y - topLeft.y)/size * TABLE_SIZE));
		}

		public Point toInteger(Point p){ return toInteger(p.x, p.y); }

		public void zoom(int x, int y, boolean isZoomingIn){
			Point z = toComplex(x, y);
			if(isZoomingIn) {
				size *= 2.0/3.0;
				topLeft.x = (2*topLeft.x + z.x)/3;
				topLeft.y = (2*topLeft.y + z.y)/3;
			} else {
				size *= 1.5;
				topLeft.x = (3*topLeft.x - z.x)/2;
				topLeft.y = (3*topLeft.y - z.y)/2;
			}
		}

		public void paintComponent(Graphics g) {
			super.paintComponent(g);
		}

		public void  mousePressed(MouseEvent evt) {
			if(isZoomActivated){
				zoom(evt.getX(), evt.getY(), evt.getButton() == MouseEvent.BUTTON1);
				repaint();
			}
		}

		public void mouseReleased(MouseEvent evt) { }
		public void  mouseClicked(MouseEvent evt) { }
		public void  mouseEntered(MouseEvent evt) { }
		public void   mouseExited(MouseEvent evt) { }
	}

	private class Point{
		public double x, y;
		public Point(double x, double y) {
			this.x = x;
			this.y = y;
		}

		public double abs() {
			return x*x + y*y;
		}

		public void f_c(Point c) {
			double _x = x*x - y*y + c.x;
			double _y = 2*x*y + c.y;
			x = _x;
			y = _y;
		}
	}

}
