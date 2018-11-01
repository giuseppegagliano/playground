import org.apache.spark.SparkConf
import org.apache.spark.sql.types._
import org.apache.spark.sql.{SaveMode, SparkSession}

object main {
  def main(args: Array[String]) {
    val conf = new SparkConf().setMaster("local[*]").setAppName("Simple Application")
    val spark = SparkSession.builder.config(conf).getOrCreate()


    val schema= StructType(Array(
      // i,sa,inv_sa,lcp,wordindex,buf+sa
      StructField("i", IntegerType,true),
      StructField("sa", IntegerType,true),
      StructField("lcp", IntegerType,true),
      StructField("wordindex", IntegerType,true),
      StructField("buf+sa", StringType,true)
    ));

    val df = spark.read.
      format("com.databricks.spark.csv")
      .schema(schema)
      .option("header","true")
      .load("/tmp/glcr_cache.gsa");


    df.write.mode(SaveMode.Overwrite).parquet("/tmp/glcr_parquet")

    spark.stop()
  }
}