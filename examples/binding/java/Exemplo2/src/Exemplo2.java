/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



import br.inpe.terralib5.DataSet;
import br.inpe.terralib5.DataSource;
import br.inpe.terralib5.QtInitializer;
import br.inpe.terralib5.Str2StrMap;
import br.inpe.terralib5.StrVector;
import br.inpe.terralib5.TerraLib;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author FredBede
 */
public class Exemplo2 {

    static {
        // Loading T5Java dll
        System.loadLibrary("terralib_mod_binding_java");
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here

        // Initialize TerraLib and Qt.
        QtInitializer qt = new QtInitializer();
        qt.begin();

        try {
            TerraLib.Initialize();
            
            // Getting data source
            Str2StrMap info = new Str2StrMap();
            info.set("URI", "D:/funcate/vs2010/terralib5/munic_shp");
            DataSource src = TerraLib.MakeDataSource("OGR", info);
            
            // Getting data set names
            StrVector dsets = src.getDataSetNames();

            // Getting the first data set
            if(!dsets.isEmpty()) {
                String dsetName = dsets.get(0);
                
                // Getting data set
                DataSet dset = src.newDataSet(dsetName);
                
                // Move to the first position.
                dset.moveFirst();
                
                // Number of columns of the data set
                long ncols = dset.getNumProperties();
                
                // Print header
                for(int c = 0; c < ncols; c++) 
                    if(dset.getPropertyDataType(c) != TerraLib.GEOMETRY_TYPE)
                        System.out.print(dset.getPropertyName(c) + " | ");
                
                System.out.println();
                System.out.println(" ------------------------------------------- ");

                do {
                    // Print data
                    for(int c = 0; c < ncols; c++) 
                        if(dset.getPropertyDataType(c) != TerraLib.GEOMETRY_TYPE)
                            System.out.print(dset.getAsString(c) + " | ");

                    System.out.println();
                } while (dset.moveNext());
            }
                    
        } catch (IOException ex) {
            Logger.getLogger(Exemplo2.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        qt.end();
        
        TerraLib.Finalize();
    }
    
}
