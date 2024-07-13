#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetLineWidth(1);
	ofEnableDepthTest();

	auto radius = 8;
	auto x_span = radius * sqrt(3);
	auto flg = true;
	for (float y = 0; y < 720; y += radius * 1.5) {

		for (float x = 0; x < 720; x += x_span) {

			glm::vec3 location;
			if (flg) {

				location = glm::vec3(x, y, 0);
			}
			else {

				location = glm::vec3(x + (x_span / 2), y, 0);
			}

			this->location_list.push_back(location);
			this->life_list.push_back(100);
		}
		flg = !flg;
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	auto noise_location = glm::vec3(
		ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.03), 0, 1, 120, 600),
		ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.03), 0, 1, 120, 600),
		0);

	for (int i = 0; i < this->location_list.size(); i++) {

		if (glm::distance(this->location_list[i], noise_location) < 35) {

			this->life_list[i] = 1;
		}
		else {

			this->life_list[i] = this->life_list[i] < 100 ? this->life_list[i] + 1 : 100;
		}
	}


	for (int i = 0; i < this->location_list.size(); i++) {

		auto height = 5.f;
		if (this->life_list[i] > 60) {

			this->setHexagonToMesh(this->face, this->frame, this->location_list[i], 8, 150);
		}
		else if (this->life_list[i] > 0) {

			this->setHexagonToMesh(this->face, this->frame, this->location_list[i], 8, ofMap(this->life_list[i], 0, 60, 0, 150));
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->face.draw();
	this->frame.draw();

	/*
	int start = 50;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
void ofApp::setHexagonToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height) {

	ofColor face_color = ofColor(ofMap(height, -1, 150, 255, 0));
	ofColor frame_color = ofColor(255);

	for (int deg = 90; deg < 450; deg += 60) {

		auto face_radius = radius - 0.5;
		auto face_index = face_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(location + glm::vec3(0, 0, 0));
		vertices.push_back(location + glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(location + glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), 0));
		vertices.push_back(location + glm::vec3(0, 0, height));
		vertices.push_back(location + glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), height));
		vertices.push_back(location + glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), height));

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 4);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);

		auto frame_index = frame_target.getNumVertices();

		vertices.clear();
		vertices.push_back(location + glm::vec3(0, 0, 0));
		vertices.push_back(location + glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(location + glm::vec3(radius * cos((deg + 60) * DEG_TO_RAD), radius * sin((deg + 60) * DEG_TO_RAD), 0));
		vertices.push_back(location + glm::vec3(0, 0, height));
		vertices.push_back(location + glm::vec3(radius * cos((deg + 60) * DEG_TO_RAD), radius * sin((deg + 60) * DEG_TO_RAD), height));
		vertices.push_back(location + glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height));

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 4);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}