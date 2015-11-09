mean_vel <- function(data)
{
  data1 <- data[1,]
  N <- ncol(data1)/3
  v <- c(0,0) ##Average Velocity vector of the system
  S <- c(0,0) ## Sum of velocities of all particles
  for(i in 1:N)
  {
    S <- S + c( cos(data1[1,3*i]) , sin(data1[1,3*i]) )
  }
  
  v <- S/N
  v
}

distance <- function(x,y,L = sqrt(100/4)){
  
  x <- abs(x)
  y <- abs(y)
  
  if(x > L/2) x <- L - x
  if(y > L/2) y <- L - y 
  
  d <- sqrt(x*x +y*y)
  
  d
}

correlations <- function(data,noise = 0) ## Creates correlation matrix. Note that default system length is 10. Change as required.
{#Default density used is 4. Change distance function etc according to need
  data1 <- data[5,]
  N <- ncol(data1)/3
  
  v <- mean_vel(data1)
  
  correlationtable <- matrix(rep(0,N*(N+1)*3/2),nrow = N*(N+1)/2, ncol = 3)
  correlationtablerow <- 0
  for(i in 1:(N-1) ){
    
    ui <- c( cos(data1[1,3*i]) , sin(data1[1,3*i])) - v
    
    if(ui[1] == 0 && ui[2] == 0) next()
    else{
    
    for(j in i:N){
      
      uj <- c( cos(data1[1,3*j]) , sin(data1[1,3*j])) - v
      if(uj[1] == 0 && uj[2] == 0) next() 
      else{
      dx <- data1[1,3*i-2] - data1[1,3*j-2]
      dy <- data1[1,3*i-1] - data1[1,3*j-1] 
      
      d <- distance(dx,dy,L = sqrt(N/4)) ##distance is measured along torus
      
      dot <- sum(ui*uj)
      
      correlationtablerow <- correlationtablerow + 1 
      correlationtable[correlationtablerow,1:2] <- c(d,dot) 
      
      }
    }##End of J loop
      
    }
  }##End of i loop
  
  len = max(correlationtable[,1])
  correlationtable[,3] <- cut(correlationtable[,1],50,labels = 1:50)
  correlationtable[,3] <- as.numeric(correlationtable[,3])
  y <- 2*correlationtable[,3] - 1
  correlationtable[,3] <- y
  
  corr <- tapply(correlationtable[,2],correlationtable[,3],mean)
  bins <- as.numeric(names(corr))*len/100
  c <- corr[1]
  corr <- corr/c
  
  
  plot(bins,corr,xlab = "Distance",ylab = "Correlation",main = paste("Bins_Noise =",noise),ylim = c(-1,1))
  
  
  
}



