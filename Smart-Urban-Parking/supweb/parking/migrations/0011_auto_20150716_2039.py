# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.core.validators


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0010_auto_20150618_2235'),
    ]

    operations = [
        migrations.CreateModel(
            name='BLE_Tag',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('plate_number', models.CharField(default=b'None', max_length=256, db_column=b'plate_number', blank=True)),
                ('timestamp', models.DateTimeField()),
                ('distance', models.FloatField(default=0, validators=[django.core.validators.MaxValueValidator(1000), django.core.validators.MinValueValidator(0)])),
            ],
        ),
        migrations.RemoveField(
            model_name='parking',
            name='occupancy',
        ),
        migrations.AddField(
            model_name='lamp',
            name='lamp_number',
            field=models.CharField(default=b'None', max_length=256, db_column=b'lamp_number', blank=True),
        ),
    ]
