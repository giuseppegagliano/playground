
name := "playground"

version := "0.1"

scalaVersion := "2.11.12"

resolvers += Resolver.mavenLocal

libraryDependencies ++= Seq(
  "org.apache.spark" %% "spark-core" % "2.3.0",
  "org.apache.spark" %% "spark-streaming" % "2.3.0" % "provided",
  "org.apache.spark" %% "spark-sql" % "2.3.0"
)