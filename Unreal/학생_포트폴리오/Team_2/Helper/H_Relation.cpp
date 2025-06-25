// Fill out your copyright notice in the Description page of Project Settings.


#include "H_Relation.h"

bool H_Relation::IsInSight(AActor* user, AActor* target, float maxAngle)
{
    if (!user || !target) return false;

    FVector ForwardVector = user->GetActorForwardVector(); // AI�� ���� ����
    FVector ToTarget = (target->GetActorLocation() - user->GetActorLocation()).GetSafeNormal(); // AI �� Target ���� ����

    float DotProduct = FVector::DotProduct(ForwardVector, ToTarget); // �� ������ ���� ���
    float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct)); // ������ �̿��� ���� ���

    return Angle <= maxAngle; // ������ ���� �̳����� üũ
}

bool H_Relation::IsInSightYaw(AActor* user, AActor* target, float maxAngle)
{
    if (!user || !target) return false;

    FVector ForwardVector = user->GetActorForwardVector(); // AI�� ���� ����
    FVector ToTarget = (target->GetActorLocation() - user->GetActorLocation());
    ToTarget.Z = ForwardVector.Z;
    ToTarget=ToTarget.GetSafeNormal();
    float DotProduct = FVector::DotProduct(ForwardVector, ToTarget); // �� ������ ���� ���
    float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct)); // ������ �̿��� ���� ���

    return Angle <= maxAngle; // ������ ���� �̳����� üũ
}

bool H_Relation::IsInDistance(AActor* user, AActor* target, float maxDistance,float minDistance=0.0f)
{
    if (!user || !target) return false;

    return (user->GetDistanceTo(target) <= maxDistance&& user->GetDistanceTo(target) >= minDistance); // ������ ���� �̳����� üũ
}

FVector H_Relation::LocOfDistanceFromTarget(AActor* user, AActor* target, float distance)
{
    FVector temp = user->GetActorLocation() - target->GetActorLocation();
    temp.Normalize();
    temp*= distance;
    return target->GetActorLocation()+temp;
}

FVector H_Relation::FindFloorFromLoc(AActor* user,float maxDistance, bool useAnotherLoc, FVector loc)
{
    FVector Start = useAnotherLoc?loc: user->GetActorLocation(); // ĳ���� ��ġ
    FVector End = Start - FVector(0, 0, maxDistance); // �Ʒ� �������� 1000 ����

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(user); // ĳ���� �ڽ��� �浹 �˻翡�� ����

    // ���� Ʈ���̽� ����
    bool bHit = user->GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_GameTraceChannel9, // ���� ECC_Visibility �Ǵ� ECC_WorldStatic ���
        Params
    );

    if (bHit)
    {
       

       // UE_LOG(LogTemp, Error, TEXT("Collision Target %s"), *(HitResult.GetActor()->GetName()));
        return HitResult.ImpactPoint; // �浹�� ��ġ ��ȯ (�� ��ǥ)
        
    }

       // UE_LOG(LogTemp, Error, TEXT("Collision Target None"));
    return FVector::ZeroVector; // �浹�� ������ (0,0,0) ��ȯ
}
