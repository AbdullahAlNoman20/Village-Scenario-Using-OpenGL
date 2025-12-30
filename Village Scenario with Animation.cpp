#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>


// GLOBAL VARIABLES
float sunX = -4.0f;           
float sunY = 3.5f;            
float cloudX = 4.0f;          
float cloud2X = -2.0f;        
float humanX = -3.5f;         
float boatX = -4.0f;         
float birdX = 3.0f;          
float birdY = 3.0f;         
float windOffset = 0.0f;     
float riverFlow = 0.0f;       

// BRESENHAM'S LINE ALGORITHM
void bresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while(true) {
        glVertex2i(x1, y1);
        
        if(x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if(e2 > -dy) { err -= dy; x1 += sx; }
        if(e2 < dx) { err += dx; y1 += sy; }
    }
}

// DRAW TEXT
void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for(int i = 0; text[i] != '\0'; i++) {  //For Each characters
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}


// DRAW SKY
void drawSky() {
    glBegin(GL_QUADS);
        glColor3f(0.53f, 0.81f, 0.92f);  
        glVertex2f(-5.0f, 5.0f);
        glVertex2f(5.0f, 5.0f);
        glColor3f(0.68f, 0.85f, 0.95f);  
        glVertex2f(5.0f, 0.0f);
        glVertex2f(-5.0f, 0.0f);
    glEnd();
}


// DRAW GROUND
void drawGround() {
    // Main ground
    glColor3f(0.4f, 0.3f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2f(-5.0f, -1.5f);
        glVertex2f(5.0f, -1.5f);
        glVertex2f(5.0f, -5.0f);
        glVertex2f(-5.0f, -5.0f);
    glEnd();
    
    // Grass layer
    glColor3f(0.25f, 0.55f, 0.25f); 
    glBegin(GL_QUADS);
        glVertex2f(-5.0f, -1.5f);
        glVertex2f(5.0f, -1.5f);
        glVertex2f(5.0f, -1.6f);
        glVertex2f(-5.0f, -1.6f);
    glEnd();
    
    //grass with wind effect
    for(float x = -4.8f; x < 4.8f; x += 0.15f) {
        float sway = sin(windOffset + x * 2) * 0.04f;
        
        // grass heights
        float height = 0.15f + (sin(x * 5) * 0.05f);
        
        // Dark green grass
        glColor3f(0.15f, 0.45f, 0.15f);
        glBegin(GL_TRIANGLES);
            glVertex2f(x, -1.5f);
            glVertex2f(x + 0.02f, -1.5f);
            glVertex2f(x + 0.01f + sway, -1.5f + height);
        glEnd();
        
        // Light green grass
        glColor3f(0.3f, 0.6f, 0.3f);
        glBegin(GL_LINES);
            glVertex2f(x + 0.01f, -1.5f);
            glVertex2f(x + 0.01f + sway, -1.5f + height);
        glEnd();
    }
}


// DRAW RIVER
void drawRiver() {
    // River water
    glColor3f(0.2f, 0.5f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(-5.0f, -2.5f);
        glVertex2f(5.0f, -2.5f);
        glVertex2f(5.0f, -3.5f);
        glVertex2f(-5.0f, -3.5f);
    glEnd();
    
    // River waves
    glColor3f(0.4f, 0.7f, 0.95f); 
    glLineWidth(2);
    for(int i = 0; i < 4; i++) {
        glBegin(GL_LINE_STRIP);
            for(float x = -5.0f; x <= 5.0f; x += 0.2f) {
                float y = -2.9f - i * 0.25f + 0.08f * sin((x + riverFlow + i) * 2);
                glVertex2f(x, y);
            }
        glEnd();
    }
    glLineWidth(1);
    
}


// DRAW BOAT
void drawBoat() {
    glPushMatrix();
    glTranslatef(boatX, -2.9f, 0.0f);
    
    // Boat body
    glColor3f(0.6f, 0.3f, 0.1f);  
    glBegin(GL_POLYGON);
        glVertex2f(-0.3f, 0.0f);
        glVertex2f(0.3f, 0.0f);
        glVertex2f(0.25f, -0.12f);
        glVertex2f(-0.25f, -0.12f);
    glEnd();
    
    // Boat inside
    glColor3f(0.5f, 0.25f, 0.08f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.25f, 0.0f);
        glVertex2f(0.25f, 0.0f);
        glVertex2f(0.2f, -0.08f);
        glVertex2f(-0.2f, -0.08f);
    glEnd();
    
    // Sail pole
    glColor3f(0.3f, 0.15f, 0.05f);
    glBegin(GL_QUADS);
        glVertex2f(-0.02f, 0.0f);
        glVertex2f(0.02f, 0.0f);
        glVertex2f(0.02f, 0.5f);
        glVertex2f(-0.02f, 0.5f);
    glEnd();
    
    // Sail
    glColor3f(0.95f, 0.95f, 0.9f); 
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.0f, 0.05f);
        glVertex2f(0.35f, 0.25f);
    glEnd();
    
    glPopMatrix();
}


// DRAW MAIN HOUSE
void drawMainHouse() {
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    
    // Main house walls
    glColor3f(0.85f, 0.75f, 0.6f);
    glBegin(GL_QUADS);
        glVertex2f(-1.2f, -1.0f);
        glVertex2f(1.2f, -1.0f);
        glVertex2f(1.2f, 0.8f);
        glVertex2f(-1.2f, 0.8f);
    glEnd();
    
    // Roof
    glColor3f(0.7f, 0.2f, 0.15f); 
    glBegin(GL_POLYGON);
        glVertex2f(-1.4f, 0.8f);
        glVertex2f(1.4f, 0.8f);
        glVertex2f(1.2f, 1.5f);
        glVertex2f(0.0f, 1.8f);
        glVertex2f(-1.2f, 1.5f);
    glEnd();
    
    // Roof details
    glColor3f(0.6f, 0.15f, 0.1f);
    glLineWidth(2);
    for(float y = 0.9f; y < 1.5f; y += 0.15f) {
        glBegin(GL_LINES);
            glVertex2f(-1.3f, y);
            glVertex2f(1.3f, y);
        glEnd();
    }
    glLineWidth(1);
    
    // Main door
    glColor3f(0.35f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(-0.3f, -1.0f);
        glVertex2f(0.3f, -1.0f);
        glVertex2f(0.3f, 0.3f);
        glVertex2f(-0.3f, 0.3f);
    glEnd();
    
    // Door knob
    glColor3f(0.8f, 0.7f, 0.2f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.22f, -0.35f);
        for(int i = 0; i <= 360; i += 30) {
            float rad = i * 3.1416f / 180.0f;
            glVertex2f(0.22f + 0.03f * cos(rad), -0.35f + 0.03f * sin(rad));
        }
    glEnd();
    
    //2 large windows
    for(int i = 0; i < 2; i++) {
        float xPos = -0.7f + i * 1.4f;
        
        // Window glass
        glColor3f(0.6f, 0.85f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(xPos - 0.25f, 0.4f);
            glVertex2f(xPos + 0.25f, 0.4f);
            glVertex2f(xPos + 0.25f, 0.7f);
            glVertex2f(xPos - 0.25f, 0.7f);
        glEnd();
        
        // Window frame
        glColor3f(0.4f, 0.25f, 0.15f);
        glLineWidth(3);
        glBegin(GL_LINE_LOOP);
            glVertex2f(xPos - 0.25f, 0.4f);
            glVertex2f(xPos + 0.25f, 0.4f);
            glVertex2f(xPos + 0.25f, 0.7f);
            glVertex2f(xPos - 0.25f, 0.7f);
        glEnd();
        
        // Window cross
        glBegin(GL_LINES);
            glVertex2f(xPos, 0.4f);
            glVertex2f(xPos, 0.7f);
            glVertex2f(xPos - 0.25f, 0.55f);
            glVertex2f(xPos + 0.25f, 0.55f);
        glEnd();
        glLineWidth(1);
    }
    
    // Chimney
    glColor3f(0.6f, 0.3f, 0.25f);
    glBegin(GL_QUADS);
        glVertex2f(0.7f, 1.3f);
        glVertex2f(1.0f, 1.3f);
        glVertex2f(1.0f, 1.9f);
        glVertex2f(0.7f, 1.9f);
    glEnd();
    
    // Smoke from chimney
    glColor3f(0.7f, 0.7f, 0.7f);
    for(int i = 0; i < 3; i++) {
        float offset = sin(windOffset + i) * 0.1f;
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.85f + offset, 1.9f + i * 0.15f);
            for(int angle = 0; angle <= 360; angle += 30) {
                float rad = angle * 3.1416f / 180.0f;
                float r = 0.08f + i * 0.02f;
                glVertex2f(0.85f + offset + r * cos(rad), 1.9f + i * 0.15f + r * sin(rad));
            }
        glEnd();
    }
    
    // House Name
    glColor3f(0.1f, 0.1f, 0.1f); 
    drawText(-0.5f, 0.45f, "      BMN");
    
    glPopMatrix();
}

// DRAW SMALL HOUSE
void drawSmallHouse(float x, float y, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    
    // House walls
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(-0.5f, -1.0f);
        glVertex2f(0.5f, -1.0f);
        glVertex2f(0.5f, 0.3f);
        glVertex2f(-0.5f, 0.3f);
    glEnd();
    
    // Roof
    glColor3f(0.6f, 0.25f, 0.2f); 
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.65f, 0.3f);
        glVertex2f(0.65f, 0.3f);
        glVertex2f(0.0f, 0.9f);
    glEnd();
    
    // Door
    glColor3f(0.4f, 0.25f, 0.15f); 
    glBegin(GL_QUADS);
        glVertex2f(-0.15f, -1.0f);
        glVertex2f(0.15f, -1.0f);
        glVertex2f(0.15f, -0.3f);
        glVertex2f(-0.15f, -0.3f);
    glEnd();
    
    // Window
    glColor3f(0.7f, 0.9f, 1.0f);  // Light blue
    glBegin(GL_QUADS);
        glVertex2f(-0.35f, -0.1f);
        glVertex2f(-0.05f, -0.1f);
        glVertex2f(-0.05f, 0.15f);
        glVertex2f(-0.35f, 0.15f);
    glEnd();
    
    // Window frame
    glColor3f(0.3f, 0.2f, 0.1f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.35f, -0.1f);
        glVertex2f(-0.05f, -0.1f);
        glVertex2f(-0.05f, 0.15f);
        glVertex2f(-0.35f, 0.15f);
    glEnd();
    glLineWidth(1);
    
    glPopMatrix();
}

// DRAW TREE
void drawTree(float x, float y, float size) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    
    // // Wind sway effect
    float sway = sin(windOffset + x) * 0.03f;
    
    // Tree Body
    glColor3f(0.35f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(-0.08f * size, -1.0f);
        glVertex2f(0.08f * size, -1.0f);
        glVertex2f((0.06f + sway * 0.5f) * size, 0.3f * size);
        glVertex2f((-0.06f + sway * 0.5f) * size, 0.3f * size);
    glEnd();
    
    // Tree Body texture
    glColor3f(0.25f, 0.15f, 0.08f);
    glLineWidth(2);
    for(float ty = -0.9f; ty < 0.3f * size; ty += 0.15f) {
        glBegin(GL_LINES);
            glVertex2f(-0.08f * size, ty);
            glVertex2f(0.08f * size, ty + 0.05f);
        glEnd();
    }
    glLineWidth(1);
    
    // Tree leaves
    glColor3f(0.1f, 0.5f, 0.1f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(sway * size, 0.5f * size);
        for(int i = 0; i <= 360; i += 15) {
            float th = i * 3.1416f / 180.0f;
            glVertex2f((sway + 0.55f * cos(th)) * size, 
                      (0.5f + 0.55f * sin(th)) * size);
        }
    glEnd();
    
    glPopMatrix();
}

// DRAW SUN
void drawSun() {
    glPushMatrix();
    glTranslatef(sunX, sunY, 0.0f);
    
    // Sun rays
    glColor3f(1.0f, 0.95f, 0.5f); 
    glLineWidth(3);
    for(int i = 0; i < 16; i++) {
        float angle = (i * 22.5f + windOffset * 20) * 3.1416f / 180.0f;
        glBegin(GL_LINES);
            glVertex2f(0.45f * cos(angle), 0.45f * sin(angle));
            glVertex2f(0.7f * cos(angle), 0.7f * sin(angle));
        glEnd();
    }
    glLineWidth(1);
    
    // Sun body 
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0f, 1.0f, 0.8f); 
        glVertex2f(0.0f, 0.0f);
        glColor3f(1.0f, 0.9f, 0.0f); 
        for(int i = 0; i <= 360; i += 10) {
            float th = i * 3.1416f / 180.0f;
            glVertex2f(0.4f * cos(th), 0.4f * sin(th));
        }
    glEnd();
    
    glPopMatrix();
}

// DRAW CLOUD
void drawCloud(float x, float y) {
    glColor3f(0.95f, 0.95f, 0.98f); 
    
    // Multiple circles for fluffy effect
    float sizes[] = {0.3f, 0.4f, 0.35f, 0.3f};
    float offsets[] = {-0.3f, -0.1f, 0.15f, 0.4f};
    
    for(int j = 0; j < 4; j++) {
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(x + offsets[j], y);
            for(int i = 0; i <= 360; i += 15) {
                float th = i * 3.1416f / 180.0f;
                glVertex2f(x + offsets[j] + sizes[j] * cos(th), 
                          y + sizes[j] * 0.6f * sin(th));
            }
        glEnd();
    }
}

// DRAW FLYING BIRD
void drawBird(float x, float y) {
    glColor3f(0.15f, 0.15f, 0.15f); 
    glLineWidth(2);
    
    float flap = sin(windOffset * 3) * 0.05f;
    
    glBegin(GL_LINE_STRIP);
        glVertex2f(x - 0.12f, y + 0.05f + flap);
        glVertex2f(x, y);
        glVertex2f(x + 0.12f, y + 0.05f + flap);
    glEnd();
    glLineWidth(1);
}

// DRAW HUMAN
void drawHuman(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    
    // Head
    glColor3f(1.0f, 0.85f, 0.7f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.35f);
        for(int i = 0; i <= 360; i += 20) {
            float th = i * 3.1416f / 180.0f;
            glVertex2f(0.08f * cos(th), 0.35f + 0.08f * sin(th));
        }
    glEnd();
    
    // Body
    glColor3f(0.2f, 0.4f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(-0.1f, 0.27f);
        glVertex2f(0.1f, 0.27f);
        glVertex2f(0.1f, 0.0f);
        glVertex2f(-0.1f, 0.0f);
    glEnd();
    
    // Legs
    glColor3f(0.15f, 0.15f, 0.15f); 
    float legSwing = sin(x * 15) * 0.12f;
    glLineWidth(4);
    glBegin(GL_LINES);
        // Left leg
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-0.06f + legSwing, -0.25f);
        // Right leg
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.06f - legSwing, -0.25f);
    glEnd();
    
    // Arms
    float armSwing = sin(x * 15) * 0.1f;
    glLineWidth(3);
    glBegin(GL_LINES);
        // Left arm
        glVertex2f(-0.1f, 0.22f);
        glVertex2f(-0.18f - armSwing, 0.05f);
        // Right arm
        glVertex2f(0.1f, 0.22f);
        glVertex2f(0.18f + armSwing, 0.05f);
    glEnd();
    glLineWidth(1);
    
    glPopMatrix();
}

// DRAW FENCE
void drawFence() {
    glColor3f(0.5f, 0.35f, 0.2f); 
    
    for(float x = -4.5f; x <= 4.5f; x += 0.4f) {
        // Fence post
        glBegin(GL_QUADS);
            glVertex2f(x - 0.03f, -1.5f);
            glVertex2f(x + 0.03f, -1.5f);
            glVertex2f(x + 0.03f, -0.9f);
            glVertex2f(x - 0.03f, -0.9f);
        glEnd();
    }
    
    // Horizontal fence rails
    glLineWidth(3);
    glBegin(GL_LINES);
        // Top rail
        glVertex2f(-4.5f, -1.0f);
        glVertex2f(4.5f, -1.0f);
        // Bottom rail
        glVertex2f(-4.5f, -1.3f);
        glVertex2f(4.5f, -1.3f);
    glEnd();
    glLineWidth(1);
}

// DISPLAY FUNCTION
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D(-5, 5, -5, 5);
    
    // Draw all
    drawSky();                              
    drawSun();                             
    drawCloud(cloudX, 3.2f);               
    drawCloud(cloud2X, 3.8f);               
    drawBird(birdX, birdY);                
    
    drawGround();                           
    drawRiver();                            
    drawBoat();                             
    
    // Draw All trees 
    drawTree(-4.0f, -1.5f, 1.2f);          
    drawTree(-2.8f, -1.5f, 0.9f);           
    drawTree(3.2f, -1.5f, 1.0f);           
    drawTree(4.5f, -1.5f, 0.8f);          
    
    // Draw houses 
    drawMainHouse();                        
    drawSmallHouse(-2.5f, -0.5f, 0.88f, 0.72f, 0.55f);  
    drawSmallHouse(2.8f, -0.5f, 0.82f, 0.65f, 0.48f);   
    
    drawFence();                            
    drawHuman(humanX, -1.25f);             
    
    glFlush();
    glutSwapBuffers();
}

// ANIMATION FUNCTION
void update(int value) {
    // Sun movement 
    sunX += 0.01f;
    if(sunX > 5.5f) {
        sunX = -5.5f;
        sunY = 3.5f;
    }
    // Sun rises and sets 
    if(sunX < 0) sunY = 3.2f + sunX * 0.2f;
    else sunY = 3.2f - sunX * 0.2f;
    
    // Cloud movements 
    cloudX -= 0.006f;
    if(cloudX < -6.5f) cloudX = 6.5f;
    
    cloud2X -= 0.004f;
    if(cloud2X < -6.5f) cloud2X = 6.5f;
    
    // Human walking 
    humanX += 0.012f;
    if(humanX > 5.5f) humanX = -5.5f;
    
    // Boat floating 
    boatX += 0.01f;
    if(boatX > 5.8f) boatX = -5.5f;
    
    // Bird flying 
    birdX -= 0.018f;
    birdY = 3.3f + sin(birdX * 2) * 0.4f;
    if(birdX < -5.5f) birdX = 5.5f;
    
    // Wind and 
    windOffset += 0.01f;
    riverFlow += 0.01f;
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// MAIN FUNCTION

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Village Scenario with Animation");
    
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    
    glutMainLoop();
    return 0;
}