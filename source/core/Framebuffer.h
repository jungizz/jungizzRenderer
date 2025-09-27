#pragma once
#include <QOpenGLFunctions>

struct FBO : protected QOpenGLFunctions {
    GLuint framebuffer = 0;
    GLuint colorTexBuffer = 0;
    GLuint depthBuffer = 0;
    int width = 0, height = 0;

    GLint prevFramebuffer; // 현재 바인딩된 FBO id를 저장 -> qt의 default fbo로 돌리기 위해

    void init(int w, int h) {
        initializeOpenGLFunctions();
        width = w; height = h;

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &colorTexBuffer);
        glBindTexture(GL_TEXTURE_2D, colorTexBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // 이미지의 데이터를 로드하는게 아니니까 마지막 인자가 null
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexBuffer, 0); // attachment

        glGenTextures(1, &depthBuffer);
        glBindTexture(GL_TEXTURE_2D, depthBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0); // attachment
        // 이건 기존 코드고, 아래는 지피티가 알려준건데.. 뭔가 배웠던거같음 강노 찾아보기

        // glGenRenderbuffers(1, &depthBuffer);
        // glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer, 0); // attachment

        GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);
    }

    void resize(int w, int h){ // 기존 코드에서 이걸 안해줘서.. 창 크기 바꾸는게 안됐던거아닐까 (아직 적용 안함)
        cleanup(); // delete old
        init(w, h);
    }

    void cleanup() {
        if (colorTexBuffer) { glDeleteTextures(1, &colorTexBuffer); colorTexBuffer = 0; }
        //if (depthBuffer) { glDeleteRenderbuffers(1, &depthBuffer); depthBuffer = 0; }
        if (depthBuffer) { glDeleteTextures(1, &depthBuffer); depthBuffer = 0; }
        if (framebuffer) { glDeleteFramebuffers(1, &framebuffer); framebuffer = 0; }
    }

    void bind() { 
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); 
        //qDebug() << "[bind] prevFramebuffer: " << prevFramebuffer << " framebuffer: " << framebuffer;
    }
    void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);}
};
