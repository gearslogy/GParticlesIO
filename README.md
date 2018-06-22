* GParticlesIO
   * Email:liuyangping207@qq.com

* can do:
    * This GIO can transfer particles data in several 3d applications.
        * examples:
        * (1)Export Houdini Particles --> IntoDisk --> named .gp cache format.
        * (2)Arnold Load The cache --> FromDisk --> use the procedural plugin to render the attributes.
        * (3)Maya Load the cache --> FromDisk --> use the rewrite the emitter load the particle cache.

* demos:
    * 1,houdini load and save data demo (Houdini_SRC),this demo can write all the Houdini point attributes into local disk.and load it
    back !
    * 2,houdini write this data,I want load in arnold Procedural,So i give the demo(Arnold_SRC).it can render Attributes(INT,
	FLOAT,Vector Int,Vector FLT)that from the Houdini.

* limitations:
	Current do not support the matrix attribute.
	

* Houdini ROP/Load:
![image](https://raw.githubusercontent.com/gearslogy/GParticlesIO/master/HoudiniGIO.jpg)


* Arnold
![image](https://raw.githubusercontent.com/gearslogy/GParticlesIO/master/Anold_Gio.jpg)
