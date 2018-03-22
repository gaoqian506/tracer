/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tracer.cpp
 * Author: gq
 * 
 * Created on March 21, 2018, 11:10 AM
 */

#include "tracer/tracer.h"
//#include <sutil.h>
#include <optixu/optixu_math_stream_namespace.h>
#include "tracer/commonStructs.h"

//#include <GL/glew.h>
#include <GL/glut.h>
//#include <GL/gl.h>


Tracer* g_tracer;
void glutDisplay();


Tracer::Tracer() {
    
    width_ = 512;
    height_ = 512;
    g_tracer = this;
    createContext();
//    glutInitialize( &argc, argv );
    glutInitialize();
    
    
    

    
}


Tracer::~Tracer() {
}


void Tracer::trace(std::string objfile) {
    
    OptiXMesh mesh;
    mesh.context = context_;
    loadMesh(objfile, mesh);
    
    optix::Aabb aabb;
    aabb.set( mesh.bbox_min, mesh.bbox_max );

    optix::GeometryGroup geometry_group = context_->createGeometryGroup();
    geometry_group->addChild( mesh.geom_instance );
    geometry_group->setAcceleration( context_->createAcceleration( "Trbvh" ) );
    context_[ "top_object"   ]->set( geometry_group ); 
    context_[ "top_shadower" ]->set( geometry_group );  
    
    const float max_dim = fmaxf(aabb.extent(0), aabb.extent(1)); // max of x, y components

    BasicLight lights[] = {
        { optix::make_float3( -0.5f,  0.25f, -1.0f ), optix::make_float3( 0.2f, 0.2f, 0.25f ), 0, 0 },
        { optix::make_float3( -0.5f,  0.0f ,  1.0f ), optix::make_float3( 0.1f, 0.1f, 0.10f ), 0, 0 },
        { optix::make_float3(  0.5f,  0.5f ,  0.5f ), optix::make_float3( 0.7f, 0.7f, 0.65f ), 1, 0 }
    };
    lights[0].pos *= max_dim * 10.0f; 
    lights[1].pos *= max_dim * 10.0f; 
    lights[2].pos *= max_dim * 10.0f; 

    optix::Buffer light_buffer = context_->createBuffer( RT_BUFFER_INPUT );
    light_buffer->setFormat( RT_FORMAT_USER );
    light_buffer->setElementSize( sizeof( BasicLight ) );
    light_buffer->setSize( sizeof(lights)/sizeof(lights[0]) );
    memcpy(light_buffer->map(), lights, sizeof(lights));
    light_buffer->unmap();

    context_[ "lights" ]->set( light_buffer );
    
    bake(mesh);
   
}

void Tracer::load(std::string objfile) {
    
    OptiXMesh mesh;
    mesh.context = context_;
    loadMesh(objfile, mesh);
    
    optix::Aabb aabb;
    aabb.set( mesh.bbox_min, mesh.bbox_max );

    optix::GeometryGroup geometry_group = context_->createGeometryGroup();
    geometry_group->addChild( mesh.geom_instance );
    geometry_group->setAcceleration( context_->createAcceleration( "Trbvh" ) );
    context_[ "top_object"   ]->set( geometry_group ); 
    context_[ "top_shadower" ]->set( geometry_group );  
    
    const float max_dim = fmaxf(aabb.extent(0), aabb.extent(1)); // max of x, y components

    BasicLight lights[] = {
        { optix::make_float3( -0.5f,  0.25f, -1.0f ), optix::make_float3( 0.2f, 0.2f, 0.25f ), 0, 0 },
        { optix::make_float3( -0.5f,  0.0f ,  1.0f ), optix::make_float3( 0.1f, 0.1f, 0.10f ), 0, 0 },
        { optix::make_float3(  0.5f,  0.5f ,  0.5f ), optix::make_float3( 0.7f, 0.7f, 0.65f ), 1, 0 }
    };
    lights[0].pos *= max_dim * 10.0f; 
    lights[1].pos *= max_dim * 10.0f; 
    lights[2].pos *= max_dim * 10.0f; 

    optix::Buffer light_buffer = context_->createBuffer( RT_BUFFER_INPUT );
    light_buffer->setFormat( RT_FORMAT_USER );
    light_buffer->setElementSize( sizeof( BasicLight ) );
    light_buffer->setSize( sizeof(lights)/sizeof(lights[0]) );
    memcpy(light_buffer->map(), lights, sizeof(lights));
    light_buffer->unmap();

    context_[ "lights" ]->set( light_buffer );
}

void Tracer::run() {
    
    // Initialize GL state                                                            
    glMatrixMode(GL_PROJECTION);                                                   
    glLoadIdentity();                                                              
    glOrtho(0, 1, 0, 1, -1, 1 );                                                   

    glMatrixMode(GL_MODELVIEW);                                                    
    glLoadIdentity();                                                              

    glViewport(0, 0, width_, height_);                                 

    glutShowWindow();                                                              
    glutReshapeWindow( width_, height_);

    // register glut callbacks
    glutDisplayFunc( Tracer::glutDisplay );
    //glutIdleFunc( glutDisplay );
    //glutReshapeFunc( glutResize );
    //glutKeyboardFunc( glutKeyboardPress );
    //glutMouseFunc( glutMousePress );
    //glutMotionFunc( glutMouseMotion );

    //registerExitHandler();

    glutMainLoop();
    
    
}

void Tracer::show() {
    
}

void Tracer::bake(const OptiXMesh& mesh) {
    
//    updateCamera();
    context_->launch( 0, width_, height_ );
    
}

void Tracer::launch() {
    context_->launch( 0, width_, height_ );
}

void Tracer::createContext() {
    
    bool use_pbo = false;
    
    context_ = optix::Context::create();
    context_->setRayTypeCount( 2 );
    context_->setEntryPointCount( 1 );
    
    context_["radiance_ray_type"]->setUint( 0u );
    context_["shadow_ray_type"  ]->setUint( 1u );
    context_["scene_epsilon"    ]->setFloat( 1.e-4f );   
    
    optix::Buffer buffer = sutil::createOutputBuffer( context_, RT_FORMAT_UNSIGNED_BYTE4, width_, height_, use_pbo );
    context_["output_buffer"]->set( buffer );
    
   // Ray generation program
    //const char *ptx = sutil::getPtxString( "tracer", "pinhole_camera.cu" );
    //optix::Program ray_gen_program = context_->createProgramFromPTXString( ptx, "pinhole_camera" );
    optix::Program ray_gen_program = context_->createProgramFromPTXFile("ptx/pinhole_camera.ptx", "pinhole_camera" );
    context_->setRayGenerationProgram( 0, ray_gen_program );    
 
    // Exception program
    optix::Program exception_program = context_->createProgramFromPTXFile( "ptx/pinhole_camera.ptx", "exception" );
    context_->setExceptionProgram( 0, exception_program );
    context_["bad_color"]->setFloat( 1.0f, 0.0f, 1.0f );  
    
    // Miss program
    //ptx = sutil::getPtxString( "tracer", "constantbg.cu" );
    //context_->setMissProgram( 0, context_->createProgramFromPTXString( ptx, "miss" ) );
    optix::Program miss_program = context_->createProgramFromPTXFile( "ptx/constantbg.ptx", "miss" );
    context_["bg_color"]->setFloat( 0.34f, 0.55f, 0.85f );
}

void Tracer::glutInitialize() {
    
    int argc = 1;
    //char argv[] = "hello" ;
    glutInit( &argc, 0 );
    glutInitDisplayMode( GLUT_RGB | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( width_, height_ );
    glutInitWindowPosition( 100, 100 );                                               
    glutCreateWindow( "abcdef" );
    glutHideWindow(); 
}

optix::Buffer Tracer::getOutputBuffer() {
    return context_[ "output_buffer" ]->getBuffer();
}

void Tracer::glutDisplay() {
    //updateCamera();
    g_tracer->launch();
    

    sutil::displayBufferGL( g_tracer->getOutputBuffer() );

//    {
//      static unsigned frame_count = 0;
//      sutil::displayFps( frame_count++ );
//    }

    glutSwapBuffers();
}


/*
 
     
    //   loader.load(objfile);
//    objfile = "aaa";
 
 */