autocorrelation <- function(data,eta){
N = ncol(data)/3
T = nrow(data)

theta <- data[,seq(3,N*3,3)]

vx <- cos(theta)
vy <- sin(theta)

vxmean = rowMeans(vx)
vymean = rowMeans(vy)

ux <- vx - vxmean
uy <- vy - vymean

meanacf = numeric(length = 101)

for(n in 1:N){
  
   meanacf = meanacf + acf(ux[,n],lag.max = 100,plot = FALSE)[[1]] + acf(uy[,n],lag.max = 100,plot = FALSE)[[1]]
  
}

meanacf = meanacf/(2*N)

plot(1:101,meanacf,xlab = "Lag",ylab = "Mean Autcorrelation",ylim = c(-.5,1),main = paste("Noise = ",eta,sep = ""),type = 'l')
abline(h = 0)
}


x = list.files("/backup/Viscek Model/Correlations/Density_4/Data/",full.names = TRUE)
x <- x[3:24]

for(i in 3:24){
  
  file_name = paste("Noise_",.25*(i-1),".jpeg",sep = "")
  data = read.csv(x[i-2])
  jpeg(filename = file_name)
  autocorrelation(data,.25*(i-1))
  dev.off()
}
