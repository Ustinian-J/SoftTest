/*************************************************************************
    > File Name: temp.java
    > Author: jiangxiwei 
    > Created Time: Sat 28 Nov 2020 12:37:59 PM PST
 ************************************************************************/

/*
 * 程序名称：Apache_math
 * 程序功能：计算向量的和、两个差和两个向量的距离
 * 说明：用输入的数字决定选择的功能
 */
package apache;

import java.util.Scanner;

import org.apache.commons.math3.linear.ArrayRealVector;
import org.apache.commons.math3.linear.RealVector;

public class Apache_math {
	// evaluate sum
	public static RealVector VectorSum() {
		System.out.println("请输入元素个数：");
		Integer count;
		Scanner t = new Scanner(System.in);
		count = t.nextInt();
		
		double[] vector = {0,0,0,0}; 
		RealVector vector_Sum = new ArrayRealVector(vector);
		System.out.println("请依次输入"+count+"个向量！");
		Scanner vectorIn = new Scanner(System.in);
		while(count>0) {
			double[] vectorTemp = new double[4];		
			for(int i=0;i<4;i++) {
				vectorTemp[i] = new Double(vectorIn.nextDouble());
			}
			RealVector vector_temp = new ArrayRealVector(vectorTemp);
			vector_Sum = vector_Sum.subtract(vector_temp);
			count--;
		}
		vectorIn.close();
		t.close();
		return vector_Sum;
	}
	// evaluate minus
	public static RealVector VectorMinus() {
		System.out.println("请依次输入2个向量！");
		Scanner vectorIn = new Scanner(System.in);
		double[] value1 = new double[4];
		for(int i=0;i<4;i++) {
			value1[i] = vectorIn.nextDouble();
			
		}
		RealVector Vector1 = new ArrayRealVector(value1);
		
		double[] value2 = new double[4];
		for(int i=0;i<4;i++) {
			value2[i] = vectorIn.nextDouble();
		}
		RealVector Vector2 = new ArrayRealVector(value2);
		vectorIn.close();
		return Vector1.subtract(Vector2);
	}
	//evaluate distance
	public static double Length() {
		RealVector minusVector = VectorMinus();
		double[] numbers = minusVector.toArray();
		double length = 0;
		for (double ff:numbers) {
			length = length + Math.pow(ff, 2);
		}
		length = Math.sqrt(length);
		return length;
	}
	
	public static void main(String[] args) {
		System.out.println("功能选择（1-3）：");
		System.out.println("1.向量求和");
		System.out.println("2.两个向量求差");
		System.out.println("3.两个向量之间的距离");
		System.out.println("请输入功能序号：");
		Integer num;
		Scanner s = new Scanner(System.in);
		num = s.nextInt();
		
		switch(num) {
		case 1:
			RealVector SumResult = VectorSum();
			System.out.println("The sum of Vectors:"+SumResult.toString());
			break;
		case 2:
			RealVector MinusResult = VectorMinus();
			System.out.println("The minus of Vectors:"+MinusResult.toString());
			break;
		case 3:
			double length = Length();
			System.out.printf("%.2f",length);
			break;
		default:
			System.out.println("Error!");
			break;
		}
		s.close();	
	}
}
