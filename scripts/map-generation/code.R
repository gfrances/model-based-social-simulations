library(gstat)
library(raster)
library(sp)
library(rgdal)

# generates an autocorrelated raster map in geotiff with defined file and size     
# rangeVariogram defines the range of the autocorrelation (i.e. 1=no autocorrelation, 25=25 cells)
# rangeValues is a vector with min and max values for the raster    
generateRaster <- function( fileName, size, rangeVariogram, rangeValues )
{
    xy <- expand.grid(1:size[1],1:size[2])
    names(xy) <- c('x','y')
    yy <- predict(gstat(formula=z~1, locations=~x+y, dummy=T, beta=c(1), model=vgm(psill=0.025, range=rangeVariogram, model='Exp'), nmax=20), xy, nsim=1)
    gridded(yy) = ~x+y
    resources <- as.matrix(yy)
    # normalised    
    resources <- (resources-min(resources))/(max(resources)-min(resources))
    # rescales to min/max params
    resources <- resources*(rangeValues[2]-rangeValues[1])
    resources <- resources + rangeValues[1]
    resourcesRaster <- raster(resources)
    writeRaster(resourcesRaster, fileName,"GTiff", datatype="INT4U", overwrite=TRUE)
}

# example
path = "../maps/"
fileName = "map.tif"
size = c(50,10)
rangeVariogram = 25
rangeValues = c(0,10)

generateRaster(paste(path,fileName), size, rangeVariogram, rangeValues)

# generation of a set of maps
rangeValue = c(0,10)

for(width in seq(10,50,40))
{
    for(height in seq(10,50,40))
    {
        for(rangeVariogram in c(1,10,25))
        {
            fileNameSplit = c(path, "r", as.character(rangeVariogram), "_s" , as.character(width), "x", as.character(height), ".tif")
            fileName = paste(fileNameSplit, collapse="")
            generateRaster(fileName, c(width, height), rangeVariogram, rangeValue)
        }
    }
}

