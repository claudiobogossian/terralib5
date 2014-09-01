/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package exemplo1;

import br.inpe.terralib5.DataSource;
import br.inpe.terralib5.Layer;
import br.inpe.terralib5.LayerVector;
import br.inpe.terralib5.QtInitializer;
import br.inpe.terralib5.Str2StrMap;
import br.inpe.terralib5.TerraLib;
import java.awt.Color;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;

/**
 *
 * @author FredBede
 */
public class Exemplo1 {

    static {
        // Loading T5Java dll
        System.loadLibrary("terralib_mod_binding_java");
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        try {
            // Initialize TerraLib and Qt.
            QtInitializer qt = new QtInitializer();
            qt.begin();
            TerraLib.Initialize();
            
            System.out.println("Generating OGR1.png");
            OGRExample1();

            System.out.println("Generating OGR2.png");
            OGRExample2();
            
            System.out.println("Generating PostGIS.png");
            PostGISExample();

            // Finalizing Qt.
            qt.end();
            
            System.out.println("Finished.");
        } catch (IOException ex) {
            Logger.getLogger(Exemplo1.class.getName()).log(Level.SEVERE, null, ex);
        }

        // Finalizing TerraLib
        TerraLib.Finalize();
    } 
    
    protected static void OGRExample1() {
        Str2StrMap info = new Str2StrMap();
        info.set("URI", "D:/funcate/data/brazil.shapefiles");
        
        GetLayersAndSaveImage("OGR", info, "OGR1.png");
    }
    
    protected static void OGRExample2() {
        Str2StrMap info = new Str2StrMap();
        info.set("URI", "D:/funcate/vs2010/terralib5/munic_shp");
        
        GetLayersAndSaveImage("OGR", info, "OGR2.png");
    }

    protected static void PostGISExample() {
        Str2StrMap info = new Str2StrMap();
        info.set("PG_HOST", "localhost");
        info.set("PG_PORT", "5432");
        info.set("PG_USER", "postgres");
        info.set("PG_PASSWORD", "postgres");
        info.set("PG_DB_NAME", "t5_data");
        info.set("PG_CONNECT_TIMEOUT", "4");
        
        GetLayersAndSaveImage("POSTGIS", info, "PostGIS.png");
    }
    
    protected static void GetLayersAndSaveImage(String type, Str2StrMap info, String fileName) {
        LayerVector ls = GetLayers(type, info);
        SaveImage(ls, fileName);
    }
    
    protected static LayerVector GetLayers(String type, Str2StrMap info) {
        DataSource src = TerraLib.MakeDataSource(type, info);
        LayerVector ls = null;
        
        if(src != null) 
          ls = TerraLib.GetDataSetsAsLayers(src);
            
        return ls;
    } 
    
    protected static void SaveImage(LayerVector ls, String fileName) {
        long size = 0;
        byte [] img;

        // Getting an image of all layers
        Layer l = ls.get(0);
        img = TerraLib.GetLayersImage(ls, l.getExtent(), 1000, 1000, size, Color.white);

        // Writing image in disk.
        ByteArrayInputStream b = new ByteArrayInputStream(img);
        Image ii;
        try {
            ii = ImageIO.read(b);
            BufferedImage bi = new BufferedImage(1000, 1000, BufferedImage.TYPE_INT_RGB);
            bi.createGraphics().drawImage(ii, 0, 0, null);
            File out = new File(fileName);
            ImageIO.write(bi, "png", out);
        } catch (IOException ex) {
            Logger.getLogger(Exemplo1.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
