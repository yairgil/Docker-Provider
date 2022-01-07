FROM openjdk:8-jdk

COPY Program.java /app/Program.java
WORKDIR /app
RUN [ "javac", "Program.java" ] 

ENV MB_LOGGED_PER_MINUTE=0.1
ENV LOG_LINE_LENGTH_BYTES=40
ENV EXIT_AFTER_N_LINES=0

CMD [ "java", "Program" ]
