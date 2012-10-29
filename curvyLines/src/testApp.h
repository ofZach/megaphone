#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "particle.h"
#include "spring.h"
#include "vectorField.h"


class blob {
    
public: 
    
    vector <particle * > particles;
    vector <spring> springs;
	
    
    
    bool rectangle_collision(float x_1, float y_1, float width_1, float height_1, float x_2, float y_2, float width_2, float height_2){
        return !(x_1 > x_2+width_2 || x_1+width_1 < x_2 || y_1 > y_2+height_2 || y_1+height_1 < y_2);
    }
    
    
    void repelFromBlob(blob & b){
        
        if (!rectangle_collision(box.x, box.y, box.width, box.height, b.box.x, b.box.y, b.box.width, b.box.height)){
            return;
        }
        
        for (int i = 0; i < b.particles.size(); i++){
            for (int j = 0; j < particles.size(); j++){
                b.particles[i]->addRepulsionForce(*particles[j], 40, 0.4);
            }
        }
    }
    
    
    void setup(){
        
        ofPoint randomPt = ofPoint(ofRandom(0,800), ofRandom(0,800));
        
        int nParticles = ofRandom(10,35);
        for (int i = 0; i < nParticles; i++){
            particle * myParticle = new particle();;
            
            float x = randomPt.x + 100 * cos ( (i / 50.0) * TWO_PI);
            float y = randomPt.y + 100 * sin ( (i / 50.0) * TWO_PI);
            
            myParticle->setInitialCondition(x,y ,0,0);
            particles.push_back(myParticle);
        }
        
        // change this to
        // for (int i = 0; i < 10; i++){
        // to see a chain
        
        for (int i = 0; i < particles.size()-1; i++){
            spring mySpring;
            mySpring.distance		= 10;
            mySpring.springiness	= 0.2f;
            mySpring.particleA =  (particles[i  ]);
            mySpring.particleB =  (particles[(i+1) % particles.size()]);
            springs.push_back(mySpring);
        }
        
        
    }
    
    void resetForces(){
        for (int i = 0; i < particles.size(); i++){
            particles[i]->resetForce();
        }
    }
    
    void addInternalForces(){
        for (int i = 0; i < particles.size(); i++){
            
            particles[i]->addRepulsionForce(ofGetMouseX(), ofGetMouseY(), 200, 0.7f);
            
            for (int j = 0; j < i; j++){
                particles[i]->addRepulsionForce(*particles[j], 100, 0.1);
            }
        }
        
        for (int i = 0; i < springs.size(); i++){
            springs[i].update();
        }
    }
    
    void update(){
        for (int i = 0; i < particles.size(); i++){
            particles[i]->bounceOffWalls();
            particles[i]->addDampingForce();
            particles[i]->update();
        }
        
        ofPoint min;
        ofPoint max;
        
        for (int i = 0; i < particles.size(); i++){
            if (i == 0) min.set(particles[i]->pos);
            if (i == 0) max.set(particles[i]->pos);
            
            min.x = MIN(min.x, particles[i]->pos.x);
            min.y = MIN(min.y, particles[i]->pos.y);
            max.x = MAX(max.x, particles[i]->pos.x);
            max.y = MAX(max.y, particles[i]->pos.y);
            
        }
        
        box.x = min.x;
        box.y = min.y;
        box.width = max.x - min.x;
        box.height = max.y - min.y;
    
        box.x -= 10;
        box.y -= 10;
        box.width += 20;
        box.height += 20;
        
        
    }
    
    
    
    void draw(){
        for (int i = 0; i < particles.size(); i++){
            particles[i]->draw();
        }
        
        for (int i = 0; i < springs.size(); i++){
            springs[i].draw();
        }
        
        //ofNoFill();
        //ofRect(box);
        //ofFill();
    }
    
    
    
    ofRectangle box;
    
};




class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void keyReleased (int key);
		
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
    vectorField VF;
    
        vector < blob > blobs;
    
    
    float prevMouseX;
    float prevMouseY;
	
};

#endif
	
