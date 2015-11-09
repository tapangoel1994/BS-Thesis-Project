##Calculates mean direction of motion of particles. Returns angle between -pi and pi
mean_dir <- function(data,k){
  
  sin <- 0
  cos <- 0
  for(i in seq(3,300,3)){
    
    sin <- sin + sin(data[k,i])
    cos <- cos + cos(data[k,i])
    
  }
  
  atan2(sin,cos)
}

#Calculates order parameter
OP <- function(data){
  
  sin <- 0
  cos <- 0
  for(i in seq(3,300,3)){
    
    sin <- sin + sin(data[,i])
    cos <- cos + cos(data[,i])
  }
  
  OrderParam <- (sin*sin + cos*cos)^(0.5)
  OrderParam <- OrderParam/100
  OrderParam
}

distance <- function(x,y,L){
  
  x1 <- abs(x)
  y1 <- abs(y)
  
  if(x1 > L/2) x1 <- L - x1
  if(y1 > L/2) y1 <- L - y1
  
  d <- sqrt(x1*x1 + y1*y1)
  
  d
}

correlations <- function(data){
  ##Calculate correlation in direction fluctuations. Since
  ##the interactions only affect the direction of motion, we only look at direction 
  ##fluctuations - it wouldn't make sense to look at velocity fluctautions since that has moreinfo
  ##than the system dynamics allows
  
  avg_dir <- mean_dir(data,1)
  
  correlationdata <- matrix(rep(0,401*200*3),nrow = 401*200,ncol = 3)
  k <- 1
  colnames(correlationdata) <- c("Distance","Correlation","Bins")
  for(i in 1:400){
    
    ui_x <- cos(data[1,3*i]) - cos(avg_dir)
    ui_y <- sin(data[1,3*i]) - sin(avg_dir)
    ui_dir <- atan2(ui_y,ui_x)
    
    for(j in i:400){
      uj_x <- cos(data[1,3*j]) - cos(avg_dir)
      uj_y <- sin(data[1,3*j]) - sin(avg_dir)
      uj_dir <- atan2(uj_y,uj_x)
      
     d <- distance(data[1,3*i-2]-data[1,3*j-2],data[1,3*i-1]-data[1,3*j-1],sqrt(100/3.25))
     dot <- cos(ui_dir)*cos(uj_dir) + sin(ui_dir)*sin(uj_dir)
     
     correlationdata[k,] <- c(d,dot,0)
     k <- k+1
    }
  }
  
  len <- max(correlationdata[,1])
  x <- seq(len/100,99*len/100,len/50)
  correlationdata[,3] <- cut(correlationdata[,1],50)
  x <- sort(unique(correlationdata[,3]))
  x <- len/100 + (x-1)*len/50
  plot(x,tapply(correlationdata[,2],(correlationdata[,3]),function(x) mean(x,na.rm = T)),xlab = 'distance',ylab = 'Correlation',ylim = c(-1,1))
 
   
}